import concurrent.futures as cf
import time
from typing import Callable, List, Tuple, Union

import pandas as pd
from tqdm import tqdm

from .authentication import Authentication
from .entity import Entity
from .model import Request
from .service import Service


class Benchmark:
    """
    Class to execute multiple services in parallel and allows for easy comparison of their outputs using the same input.

    Examples::

        from elg import Benchmark

        # A benchmark can be initialized with a list of services ids (from_ids method) or with a list of entities (from_entities method).
        # Here we compare English to German Machine Translation services.
        ben = Benchmark.from_ids([610, 624])

        # The benchmark can be run on multiple inputs and can be run multiple times to guarantee the result (the first run is also
        # usually longer than the next ones due to the service pods initialization).
        result = ben(["Bush is the president of the USA and lives in Washington.", "ELG is an amazing project."], number_of_runs=2)

        # The benchmark call returns a benchmark result object that can be used to compare the result.

        # You can have an overview of the result,
        df = result.compare()
        print("General comparison:\\n", df)

        # compare only the results,
        df = result.compare_results()
        print("Comparison of the results:\\n", df)

        # or only the response time.
        df = result.compare_response_times()
        print("Comparison of the response time:\\n", df)

        # The compare methods return a DataFrame object that can be exported to csv, excel and many other formats to have a
        # better visualization
        result.compare().to_csv("/tmp/result.csv")

        # We can take another example and compare sentiment analysis services.
        ben = Benchmark.from_ids([477, 510])
        inputs = [
            "This movie is not good at all.",
            "This movie is not good but it was a good moment at the cinema.",
            "This movie is not so bad.",
            "I liked the movie but it was not must seen.",
            "It was the best movie I have ever seen."
        ]
        result = ben(
            inputs,
            output_funcs=[
                lambda x: x.features["OVERALL"],
                lambda x: x.annotations["SentenceSet"][0].features["score"] * 100
            ]
        )

        print("Result:\\n")
        result.compare()
    """

    def __init__(self, services: List[Service]):
        """
        Init a Benchmark with the services to compare

        Args:
            services (List[Service]): list of the services to compare.
        """
        for service in services:
            assert isinstance(service, Service)
        self.services = services

    @classmethod
    def from_entities(
        cls,
        entities: List[Entity],
        auth_object: str = None,
        auth_file: str = None,
        scope: str = None,
        use_cache: bool = True,
        cache_dir="~/.cache/elg",
    ):
        """
        Class method to init a Benchmark using a list of entities which will be convert into services
        using the `from_entity` class method of the Service class. Refer to this method for further explanation.
        """
        return cls(
            [
                Service.from_entity(
                    entity=entity,
                    auth_object=auth_object,
                    auth_file=auth_file,
                    scope=scope,
                    use_cache=use_cache,
                    cache_dir=cache_dir,
                )
                for entity in entities
            ]
        )

    @classmethod
    def from_ids(
        cls,
        ids: List[int],
        auth_object: Authentication = None,
        auth_file: str = None,
        scope: str = None,
        domain: str = None,
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
    ):
        """
        Class method to init a Benchmark using a list of ids which will be convert into services
        using the `from_id` class method of the Service class. Refer to this method for further explanation.
        """
        return cls(
            [
                Service.from_id(
                    id=id,
                    auth_object=auth_object,
                    auth_file=auth_file,
                    scope=scope,
                    domain=domain,
                    use_cache=use_cache,
                    cache_dir=cache_dir,
                )
                for id in ids
            ]
        )

    def __call__(
        self,
        request_inputs: Union[str, List[str], Request, List[Request]] = None,
        request_type: str = "text",
        sync_mode: bool = False,
        timeout: int = None,
        check_file: bool = True,
        output_funcs: Union[str, Callable, List[Union[str, Callable]]] = "auto",
        number_of_runs: int = 2,
    ):
        """
        Method to run the comparison of the services with the given inputs.

        Args:
            request_inputs (Union[str, List[str], Request, List[Request]], optional): list of inputs on which to compare the services.
                Each input must correspond to the request_input parameter of the Service __call__ method. Defaults to None.
            request_type (str, optional): precise the type of the request. Can be "text", "structuredText", or "audio".
                It is only used if request_input is not a Request object. Defaults to "text".
            sync_mode (bool, optional): sync_mode parameter to give to the Service __call__ method. Defaults to False.
            timeout (int, optional): timeout parameter to give to the Service __call__ method. Defaults to None.
            check_file (bool, optional): check_file parameter to give to the Service __call__ method. Defaults to True.
            output_funcs (Union[str, Callable, List[Union[str, Callable]]], optional): output_func parameters to give to the Services
                __call__ method. Defaults to "auto".
            number_of_runs (int, optional): number of times to run the services on each input. It is recommended to run the services
                at least 2 times because on the first time the services usually need to be loaded in the ELG cluster, which
                will increase the response time. The response time of the second pass is, therefore, more precise. Defaults to 2.

        Returns:
            BenchmarkResult: result of the Benchmark call. To obtain the pandas DataFrame containing all the results, run the compare
                method on the obtained BenchmarkResult object.
        """
        if not isinstance(request_inputs, List):
            request_inputs = [request_inputs]

        if isinstance(output_funcs, str) and output_funcs != "auto":
            raise ValueError(
                "output_funcs must be set to 'auto', a callable object, or a list of 'auto' or callable objects with the same length as the number of services in self.services."
            )
        elif isinstance(output_funcs, str) or isinstance(output_funcs, Callable):
            output_funcs = [output_funcs for _ in range(len(self.services))]
        elif isinstance(output_funcs, List):
            if len(output_funcs) != len(self.services):
                raise ValueError(
                    "output_funcs must be set to 'auto', a callable object, or a list of 'auto' or callable objects with the same length as the number of services in self.services."
                )
            for output_func in output_funcs:
                if not ((isinstance(output_func, str) and output_func == "auto") or isinstance(output_func, Callable)):
                    raise ValueError(
                        "output_funcs must be set to 'auto', a callable object, or a list of 'auto' or callable objects with the same length as the number of services in self.services."
                    )
        else:
            raise ValueError(
                "output_funcs must be set to 'auto', a callable object, or a list of 'auto' or callable objects with the same length as the number of services in self.services."
            )

        benchmark_result = BenchmarkResult(services=self.services, request_inputs=request_inputs)

        for request_input in tqdm(request_inputs, desc="Input strs"):
            for run_idx in tqdm(range(number_of_runs), desc="Number of runs"):
                with cf.ThreadPoolExecutor() as executor:
                    futures = []
                    for output_func, service in zip(output_funcs, self.services):
                        futures.append(
                            executor.submit(
                                self._call_service,
                                service=service,
                                request_input=request_input,
                                request_type=request_type,
                                sync_mode=sync_mode,
                                timeout=timeout,
                                check_file=check_file,
                                output_func=output_func,
                            )
                        )

                    for future in cf.as_completed(futures):
                        service, result, response_time = future.result()
                        benchmark_result._append_run(
                            service=service,
                            request_input=request_input,
                            run_idx=run_idx,
                            result=result,
                            response_time=response_time,
                        )

        benchmark_result._set_df()
        return benchmark_result

    @staticmethod
    def _call_service(service: Service, **kwargs) -> Tuple[dict, float]:
        """
        Internal method to call a service
        """
        start = time.time()
        try:
            result = service(verbose=False, **kwargs)
            return service, result, time.time() - start
        except Exception as e:
            return service, {"response": str(e)}, 0.0


class BenchmarkResult:
    """
    Class the represent the result of a Benchmark call
    """

    def __init__(self, services: List[Service], request_inputs: List[str]):
        """
        Init method that taked the list of the services compared and the inputs on which to compare the services
        """
        self.services = services
        self.id2service = {service.id: service for service in self.services}
        self.result = {service: {str(request_input): {} for request_input in request_inputs} for service in services}

    def set_colwidth(self, value: int = None):
        """
        Method to easily change the colwidth value of pandas to better vizualize the DataFrame

        Args:
            value (int, optional): value of the colwidth. Defaults to None.
        """
        pd.set_option("max_colwidth", value)

    def _append_run(
        self,
        service: Service,
        request_input: str,
        run_idx: int,
        result: str,
        response_time: float,
    ):
        """
        Internal method to append the result of a run. Used by the Benchmark call function
        """
        self.result[service][request_input][run_idx] = {
            "result": result,
            "response_time": response_time,
        }

    def _set_df(self):
        """
        Internal method to create the DataFrame when the Benchmark call is finished
        """
        self.df = pd.DataFrame.from_dict(
            {
                (service.id, str(request_input), run_idx): self.result[service][str(request_input)][run_idx]
                for service in self.result.keys()
                for request_input in self.result[service].keys()
                for run_idx in self.result[service][str(request_input)].keys()
            },
            orient="index",
        )
        self.df.index.set_names(["service", "request_input", "run"], inplace=True)

    def compare(
        self,
        columns: List[str] = ["result", "response_time"],
        func: Union[str, list, dict] = "last",
        level: str = "run",
        colwidth: int = 0,
        **agg_kwargs,
    ):
        """
        Method to compare the obtained results. It returns a pandas DataFrame object containing the comparison

        Args:
            columns (List[str], optional): colums of the DataFrame to returned. Defaults to ["result", "response_time"].
            func (Union[str, list, dict], optional): function to use for the comparison. To see all the possible function,
                please see https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.core.groupby.DataFrameGroupBy.aggregate.html?.
                Defaults to "last".
            level (str, optional): level of the comparison. The level value can be: 'service', 'request_input
            ', or 'run'. Defaults to "run".
            colwidth (int, optional): if set, will change the colwidth parameter of pandas to better vizualize the DataFrame. Defaults to 0.

        Raises:
            ValueError: error if the level parameters is not set to a correct value.

        Returns:
            pd.DataFrame: pandas DataFrame object containing the comparison
        """
        if (isinstance(colwidth, int) and colwidth > 0) or colwidth is None:
            self.set_colwidth(colwidth)

        if level == "service":
            level = ["service"]
        elif level == "request_input":
            level = ["service", "request_input"]
        elif level == "run":
            level = ["service", "request_input", "run"]
        else:
            raise ValueError("level must be set to 'service', 'request_input', or 'run'.")

        return self.df.groupby(level).agg(func=func, **agg_kwargs)[columns]

    def compare_results(
        self,
        func: Union[str, list, dict] = "last",
        level: str = "request_input",
        colwidth: int = 0,
        **agg_kwargs,
    ):
        """
        Method similar to the compare method with default parameters optimized to compare the results
        """
        return self.compare(
            columns=["result"],
            func=func,
            level=level,
            colwidth=colwidth,
            **agg_kwargs,
        )

    def compare_response_times(
        self,
        func: Union[str, list, dict] = "describe",
        level: str = "service",
        colwidth: int = 0,
        **agg_kwargs,
    ):
        """
        Method similar to the compare method with default parameters optimized to compare the response_times
        """
        return self.compare(
            columns=["response_time"],
            func=func,
            level=level,
            colwidth=colwidth,
            **agg_kwargs,
        )
