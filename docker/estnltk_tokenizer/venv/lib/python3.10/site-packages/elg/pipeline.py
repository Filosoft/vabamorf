from typing import Callable, List, Union

from .authentication import Authentication
from .entity import Entity
from .model import AudioRequest, Request, StructuredTextRequest, TextRequest
from .service import Service


class Pipeline:
    """
    Class to execute multiple services on after the other. The output of the first service is used as the input of the next one
    and so one. It is a basic approach but allows users to create complex services.

    Examples::

        from elg import Pipeline

        # A pipeline is simply a list of services and therefore can be initialized with a list of service ids or a list of entities.
        german_sentiment_analysis_pipeline = Pipeline.from_ids([607, 510])

        # To map the output of a service to the next one, we use the ouput_funcs parameter to extract the needed information
        # from the service response. In this example, we need to extract only the translated text to use it as input of the
        # sentiment analyser service. The first item of the output_funcs list corresponds to the callable object that extracts
        # the translated text from the first service output, and the second item of the list corresponds to the function apply
        # to the result of the second service. Here it is set the auto which means that the automatic content extraction will be used.
        results = german_sentiment_analysis_pipeline(
            "Ich habe diesen Film geliebt. Die Schauspieler, das Drehbuch: alles von einem Meisterwerk.",
            output_funcs=[
                lambda x: x.texts[0].content,
                "auto"
            ]
        )

        # The returned results object contains the result of each service of the pipeline.
        print("Result of the translation service: ", results[0])
        print("\\nResult of the sentiment analyser service: ", results[1])

        # With the pipeline class we can create a lot of different tools. For example, let's create a pipeline that generate an
        # German summary from an English news article. To do that, we will use a summarization service (478) and an English to
        # German Machine Translation service (610).
        pipeline = Pipeline.from_ids([478, 610])
        TEXT_TO_SUMMARIZE = '''In his new book, Fulfillment, Alec MacGillis writes of an Amazon distribution center in Sparrows Point, Maryland that sits on land once occupied by a Bethlehem Steel plant. The story underscores how dramatically the U.S. economy has transformed in recent years. Instead of making things, many of our biggest companies now distribute things made elsewhere. We've moved from an economy of production to one of dispersion. The shift from factory to fulfillment work is core to the American story right now. For the American worker, a factory job like one at Bethlehem Steel was dangerous, but it paid $30 to $40 per hour, and many stuck with it for life. At an Amazon Fulfillment Center, pay starts at $15 per hour, algorithms monitor your performance, and many workers leave soon after joining. "There's 100% turnover in the warehouses," MacGillis told me this week. "100% \every single year." Some blame the move to fulfillment work entirely on Amazon, but it didn't happen in a vacuum. American politicians helped it along by signing trade deals like NAFTA and enthusiastically welcoming China into the World Trade Organization - and doing so without sufficient safeguards. American industry then suffered the consequences, and Amazon reaped the benefits. Listen to Alec MacGillis on this week's Big Technology Podcast on Apple, Spotify, or your app of choice. The U.S. embrace of globalization flooded the country's markets with inexpensive products, and plants stateside couldn't keep up. As American factories went under or moved overseas, there was glaring a need for a company that could get the new, affordable products to Americans' doorsteps. Amazon eagerly stepped in. And its timing couldn't have been better. In January 1994, NAFTA opened up trade between U.S., Canada, and Mexico. Seven months later, Jeff Bezos founded Amazon. In 2000, Amazon launched its third-party marketplace. One year later, China joined the WTO.For workers, going from $40 to $15 per hour meant moving from an annual salary of $80,000 to one closer to $30,000. And as U.S. industry struggled, Amazon's hired 1,400 workers per day. Many American workers are now removed from the production process - employed by middlemen taking a cut - so the drop in wage is natural. MacGillis tells the story of some workers who, on the same land, worked for Bethlehem Steel and then Amazon for one-third the wage. American workers have thus taken a hard look at our political system and found it wanting. In the aftermath of the trade deals, they've been left too often to decide between unemployment and jobs at fulfillment centers, ride-hailing services, and app-based food delivery. In 2016, many workers remembered Bill Clinton signing NAFTA and backing China's entry to the WTO, and voted for Donald Trump. In 2020, as Covid raged, enough moved to Joe Biden's camp that he won the election with an amalgamation of workers and college-educated liberals, what MacGillis calls the "Amazon Coalition." In some cases, Amazon workers drawn to the Democratic party for its alliance with labor were peeing in bottles as they delivered packages to urban-dwellers drawn to the party for its social values. The underlying tension flared up recently as workers at an Amazon fulfillment center in Bessemer, Alabama pushed to unionize. Bernie Sanders and Elizabeth Warren came out strongly in support of the union, fully aware of how vital fulfillment workers are to their future. Amazon then mocked the government for failing the working class. Its executive overseeing the fulfillment centers, Dave Clark, said "We actually deliver a progressive workplace," while hailing Amazon's $15 minimum wage compared to the federal government's $7.25. The union effort in Bessemer seems en route to defeat. But even in victory, it would not have replaced the fulfillment center work with factory work. The reality facing U.S. workers today is that low-paying fulfillment jobs are often their best option. And we're likely to see more political instability if we don't find some way to generate jobs that give workers a shot at the middle class. '''

        results = pipeline(
            TEXT_TO_SUMMARIZE,
            output_funcs=[
                lambda x: " ".join([data.features["prefLabel"] for data in x.annotations["Main sentence"]]),
                lambda x: x.texts[0].content,
            ]
        )
        print("German summary: ", results[-1])

    """

    def __init__(self, services: List[Service]):
        """
        Init a Pipeline with the services to execute

        Args:
            services (List[Service]): list of the services to execute. This first service of
                the list is the first service executed.
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
        Class method to init a Pipeline using a list of entities which will be convert into services
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
        Class method to init a Pipeline using a list of ids which will be convert into services
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
        request_input: Union[str, List[str], Request] = None,
        request_types: Union[str, List[str]] = "text",
        sync_mode: bool = False,
        timeout: int = None,
        check_file: bool = True,
        verbose: bool = True,
        output_funcs: Union[str, Callable, List[Union[str, Callable]]] = "auto",
    ):
        """
        Method to run the pipeline.

        Args:
            request_input (Union[str, List[str], Request], optional): input that corresponds to the request_input parameter of
                the Service __call__ method. Defaults to None.
            request_type (str, optional): precise the type of the request. Can be "text", "structuredText", or "audio".
                It is only used if request_input is not a Request object. Defaults to "text".
            sync_mode (bool, optional): sync_mode parameter to give to the Service __call__ method. Defaults to False.
            timeout (int, optional): timeout parameter to give to the Service __call__ method. Defaults to None.
            check_file (bool, optional): check_file parameter to give to the Service __call__ method. Defaults to True.
            output_funcs (Union[str, Callable, List[Union[str, Callable]]], optional): output_func parameters to give to the Services
                __call__ method. Defaults to "auto".

        Returns:
            Union[Dict, str]: service response in JSON format or as a string if output_func returns a string.
        """
        if isinstance(request_types, str):
            request_types = [request_types for _ in range(len(self.services))]
        if len(request_types) != len(self.services):
            raise ValueError("the lenght of request_type musts be equal to the number of services.")

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

        assert len(self.services) == len(request_types)
        assert len(self.services) == len(output_funcs)

        result = request_input
        results = []
        for idx, (service, request_type, output_func) in enumerate(zip(self.services, request_types, output_funcs)):
            if request_type == "text":
                request_input = TextRequest.from_auto_content(result)
            elif request_type == "structuredText":
                request_input = StructuredTextRequest.from_auto_content(result)
            elif request_type == "audio":
                request_input = AudioRequest.from_auto_content(result)
            else:
                raise ValueError("request type does not support by the Pipeline class.")
            if idx == len(self.services) - 1:
                output_func = lambda response: response
            result = service(
                request_input=request_input,
                request_type=request_type,
                sync_mode=sync_mode,
                timeout=timeout,
                check_file=check_file,
                verbose=verbose,
                output_func=output_func,
            )
            results.append(result)
            if verbose:
                print(f"Result:\n\t{result}\n")
        return results
