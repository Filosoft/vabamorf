import json
import random
import re
import string
from pathlib import Path
from typing import Any, List, Union
from urllib.parse import urlparse

from loguru import logger

from .entity import Entity, MetadataRecordObj
from .utils import get_information
from .utils.local_installation import (DEFAULT_CONF_TEMPLATE, DOCKER_COMPOSE,
                                       EXPOSE_PORT_CONFIG, FRONTEND, GUI,
                                       GUI_CONF_TEMPLATE, HELPERS,
                                       HELPERS_TEMPLATE,
                                       HTML_INDEX_HTML_TEMPLATE,
                                       HTML_INDEX_IFRAME, HTML_INDEX_SCRIPT,
                                       I18N, I18N_CONF_TEMPLATE, LTSERVICE,
                                       LTSERVICE_SIDECAR, LTSERVICE_URL)


def _name_from_image(image):
    return re.sub("[^0-9a-zA-Z]+", "_", image)[-60:]


def _random_name(length: int = 10):
    return "".join(random.choice(string.ascii_uppercase + string.digits) for _ in range(length))


class LTServiceLocalInstallation:
    """
    Class that contains all the information to deploy an ELG-compatible service locally
    """

    def __init__(
        self,
        id: int,
        image: str,
        sidecar_image: str,
        name: str,
        image_envvars: List[str],
        full_name: str,
        port: int,
        path: str,
        gui: bool,
        gui_image: str,
        gui_port: int,
        gui_path: str,
        record: Any,
    ):
        """
        Initialize a LTServiceLocalInstallation object

        Args:
            id (int): id of the LT service
            image (str): docker image of the LT service
            sidecar_image (str): docker image of the sidecar container
            name (str): short name of the LT service used in the docker-compose file
            image_envvars (List[str]): environment variables to pass to the LT service docker container
            full_name (str): long name of the LT service used in the GUI
            port (int): port exposed by the LT service docker container
            path (str): path to the LT service endpoint
            gui (bool): boolean to indicate if yes or no the GUI should be deployed
            gui_image (str): docker image of the GUI
            gui_port (int): port exposed by the GUI docker container
            gui_path (str): path to the GUI endpoint
            record (Any): metadata record of the LT service
        """
        self.id = id
        self.image = image
        self.sidecar_image = sidecar_image
        self.name = name
        self.image_envvars = image_envvars
        self.full_name = full_name
        self.port = port
        self.path = path
        self.gui = gui
        self.gui_image = gui_image
        self.gui_port = gui_port
        self.gui_path = gui_path
        self.record = record

        self.ltservice = LTSERVICE.format(
            LTSERVICE_NAME=self.name,
            LTSERVICE_IMAGE=self.image,
            LTSERVICE_ENVVARS=str(self.image_envvars),
        )

        self.sidecarservice = (
            LTSERVICE_SIDECAR.format(LTSERVICE_NAME=self.name, SIDECAR_IMAGE=self.sidecar_image)
            if self.sidecar_image
            else ""
        )

        self.url = LTSERVICE_URL.format(
            LTSERVICE_NAME=self.name,
            LTSERVICE_PORT=self.port,
            LTSERVICE_PATH=self.path,
        )
        if self.gui:
            self.iframe = HTML_INDEX_IFRAME.format(
                LTSERVICE_ID=self.id,
                LTSERVICE_FULL_NAME=self.full_name,
                LTSERVICE_NAME=self.name,
            )
            self.script = HTML_INDEX_SCRIPT.format(
                LTSERVICE_NAME=self.name,
                GUI_NAME=_name_from_image(self.gui_image),
                GUI_PATH=self.gui_path,
            )

    @classmethod
    def from_id(
        cls,
        id: int,
        gui: bool = True,
        gui_image: str = "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
        gui_port: int = 80,
        image_envvars: List[str] = [],
        domain: str = "live",
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
    ):
        """
        Class method to init a LTServiceLocalInstallation object from the id of an LT service deployed in the ELG cluster

        Args:
            id (int): id of the LT service in the ELG cluster
            gui (bool, optional): boolean to indicate if yes or no the GUI should be deployed. Defaults to True.
            gui_image (_type_, optional): docker image of the GUI. Defaults to "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest".
            gui_port (int, optional): port exposed by the GUI docker container. Defaults to 80.
            image_envvars (List[str], optional): environment variables to pass to the LT service docker container. Defaults to [].
            domain (str, optional): domain of the ELG cluster. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".

        Returns:
            LTServiceLocalInstallation: the LTServiceLocalInstallation object created
        """
        entity = Entity.from_id(id=id, domain=domain, use_cache=use_cache, cache_dir=cache_dir)
        software_distribution = get_information(
            id=entity.id,
            obj=entity.record,
            infos=["described_entity", "lr_subclass", "software_distribution"],
        )[0]
        if software_distribution.get("private_resource"):
            logger.warning(
                "Service [{id}] is private. It cannot be deployed locally.",
                id=entity.id,
            )
            return None
        sidecar_image = software_distribution.get("service_adapter_download_location")
        image = software_distribution.get("docker_download_location")
        execution_location = urlparse(software_distribution.get("execution_location"))
        name = get_information(
            id=entity.id,
            obj=entity.record,
            infos=["service_info", "elg_execution_location_sync"],
        ).split("/")[-1]
        full_name = entity.resource_name
        port = execution_location.port
        path = execution_location.path
        gui_path = get_information(id=entity.id, obj=entity.record, infos=["service_info", "elg_gui_url"],).split(
            "/"
        )[-1]
        record = entity.record
        return cls(
            id=id,
            image=image,
            sidecar_image=sidecar_image,
            name=name,
            image_envvars=image_envvars,
            full_name=full_name,
            port=port,
            path=path,
            gui=gui,
            gui_image=gui_image,
            gui_port=gui_port,
            gui_path=gui_path,
            record=record,
        )

    @classmethod
    def from_docker_image(
        cls,
        image: str,
        execution_location: str,
        sidecar_image: str = "",
        name: str = None,
        image_envvars: List[str] = [],
        full_name: str = None,
        gui: bool = False,
        gui_image: str = "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
        gui_port: int = 80,
        gui_path: str = "",
        record: Any = {},
    ):
        """
        Class method to init a LTServiceLocalInstallation object from a docker image

        Args:
            image (str): docker image of the LT service
            execution_location (str): execution location of the LT service in the LT service docker container (e.g. http://localhost:8000/process)
            sidecar_image (str, optional): docker image of the sidecar. Defaults to "".
            name (str, optional): short name of the LT service used in the docker-compose. Defaults to None.
            image_envvars (List[str], optional): environment variables to pass to the LT service docker container. Defaults to [].
            full_name (str, optional): long name of the LT service used in the GUI. Defaults to None.
            gui (bool, optional): boolean to indicate if yes or no the GUI should be deplo. Defaults to False.
            gui_image (str, optional): docker image of the GUI. Defaults to "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest".
            gui_port (int, optional): port exposed by the GUI docker container. Defaults to 80.
            gui_path (str, optional): path to the GUI endpoint. Defaults to "".
            record (Any, optional): metadata record of the LT service. Defaults to {}.

        Returns:
            LTServiceLocalInstallation: the LTServiceLocalInstallation object created
        """
        name = name if name else _random_name()
        full_name = full_name if full_name else f"ELG Service from Docker {name}"
        execution_location = urlparse(execution_location)
        port = execution_location.port
        port = int(port) if port is not None else 80
        path = execution_location.path
        return cls(
            id=-1,
            image=image,
            sidecar_image=sidecar_image,
            name=name,
            image_envvars=image_envvars,
            full_name=full_name,
            port=port,
            path=path,
            gui=gui,
            gui_image=gui_image,
            gui_port=gui_port,
            gui_path=gui_path,
            record=MetadataRecordObj(record),
        )


class LocalInstallation:
    """
    Class that contains all the information to deploy ELG-compatible services with a small part of the ELG infrastructure locally
    """

    def __init__(self, ltservices: List[LTServiceLocalInstallation], helpers: List[str] = None):
        """
        Initialize a LocalInstallation object

        Args:
            ltservices (List[LTServiceLocalInstallation]): list of the LT services local installation to deploy
            helpers (List[str], optional): list of helpers to deploy. Currently only "temp-storage" is a valid helper and can be used to deploy a temporary storage needed for some services. Defaults to None.
        """
        self.ltservices = ltservices
        self.helpers = helpers

    @classmethod
    def from_ids(
        cls,
        ids: List[int],
        gui: bool = True,
        gui_images: Union[str, List[str]] = "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest",
        gui_ports: Union[int, List[int]] = 80,
        helpers: List[str] = None,
        images_envvars: List[List[str]] = None,
        domain: str = "live",
        use_cache: bool = True,
        cache_dir: str = "~/.cache/elg",
    ):
        """
        Class method to init a LocalInstallation object from multiple ids of LT services deployed in the ELG cluster

        Args:
            ids (List[int]): list of ids od LT services in the ELG cluster
            gui (bool, optional): boolean to indicate if yes or no the GUI should be deployed. Defaults to True.
            gui_images (Union[str, List[str], optional): docker images of the GUI. If string, the same GUI docker image will be used for all the services. Otherwise, the number of GUI docker images needs to be the same as the number of LT services deployed. Defaults to "registry.gitlab.com/european-language-grid/usfd/gui-ie:latest".
            gui_ports (Union[int, List[int]], optional): port exposed by the GUI docker container. If integer, the same port will be used for all the services. Otherwise, the number of port needs to be the same as the number of LT services deployed. Defaults to 80.
            helpers (List[str], optional): list of helpers to deploy. Currently only "temp-storage" is a valid helper and can be used to deploy a temporary storage needed for some services. Defaults to None.
            images_envvars (List[List[str]], optional): environment variables to pass to the LT services docker container. Defaults to None.
            domain (str, optional): domain of the ELG cluster. Defaults to "live".
            use_cache (bool, optional): True if you want to use cached files. Defaults to True.
            cache_dir (str, optional): path to the cache_dir. Set it to None to not store any cached files. Defaults to "~/.cache/elg".

        Returns:
            LocalInstallation: the LocalInstallation object created
        """
        if isinstance(gui_images, list) and len(gui_images) == 1:
            gui_images = gui_images[0]
        if isinstance(gui_images, str):
            gui_images = [gui_images for _ in range(len(ids))]
        elif isinstance(gui_images, list):
            if len(gui_images) != len(ids):
                raise ValueError(
                    f"You provided {len(gui_images)} GUI images and {len(ids)} service ids. These two numbers must be equal."
                )
        else:
            raise ValueError("gui_images must be a string or a list of strings.")

        if isinstance(gui_ports, list) and len(gui_ports) == 1:
            gui_ports = gui_ports[0]
        if isinstance(gui_ports, int):
            gui_ports = [gui_ports for _ in range(len(ids))]
        elif isinstance(gui_ports, list):
            if len(gui_ports) != len(ids):
                raise ValueError(
                    f"You provided {len(gui_ports)} GUI ports and {len(ids)} service ids. These two numbers must be equal."
                )
        else:
            raise ValueError("gui_ports must be a int or a list of ints.")

        if images_envvars is None:
            images_envvars = [[] for _ in range(len(ids))]
        if isinstance(images_envvars, list):
            if len(images_envvars) != len(ids):
                raise ValueError(
                    f"You provided {len(images_envvars)} images env variables and {len(ids)} service ids. These two numbers must be equal."
                )
        else:
            raise ValueError("images_envvars must be a list of the same length of the number of service ids.")

        assert len(ids) == len(gui_images)
        assert len(ids) == len(gui_ports)
        assert len(ids) == len(images_envvars)

        ltservices = [
            LTServiceLocalInstallation.from_id(
                id=ltservice_id,
                gui=gui,
                gui_image=gui_image,
                gui_port=gui_port,
                image_envvars=image_envvars,
                domain=domain,
                use_cache=use_cache,
                cache_dir=cache_dir,
            )
            for ltservice_id, gui_image, gui_port, image_envvars in zip(ids, gui_images, gui_ports, images_envvars)
        ]
        ltservices = [ltservice for ltservice in ltservices if ltservice]
        return cls(ltservices=ltservices, helpers=helpers)

    def create_docker_compose(
        self,
        expose_port: int = 8080,
        path: str = "./elg_local_installation/",
    ):
        """
        Method to generate the docker compose file and all the configuration files to deploy the LocalInstallation

        Args:
            expose_port (int, optional): port used to expose the GUI or the LT service execution server. Defaults to 8080.
            path (str, optional): path where to store the configuration files. Defaults to "./elg_local_installation/".
        """
        if self.ltservices == []:
            logger.warning("None of the services can be deployed locally. Therefore, no files will be created.")
            return
        guis_image_port = list(
            set([(ltservice.gui_image, ltservice.gui_port) for ltservice in self.ltservices if ltservice.gui])
        )
        gui = len(guis_image_port) > 0
        has_helpers = self.helpers and len(self.helpers) > 0
        if gui or has_helpers:
            guis = (
                (
                    [
                        GUI.format(GUI_NAME=_name_from_image(gui_image), GUI_IMAGE=gui_image)
                        for gui_image, _ in guis_image_port
                    ]
                    + [I18N]
                )
                if gui
                else []
            )
            helpers = []
            helpers_conf = []
            if has_helpers:
                for h in self.helpers:
                    if h in HELPERS:
                        helpers.append(HELPERS[h].format(EXPOSE_PORT=expose_port))
                        helpers_conf.append(HELPERS_TEMPLATE[h])
                    else:
                        logger.warning(f"Unknown helper {h}, ignored")
            frontend = FRONTEND.format(EXPOSE_PORT=expose_port)
            gui_conf_templates = (
                (
                    [
                        GUI_CONF_TEMPLATE.format(GUI_NAME=_name_from_image(gui_image), GUI_PORT=gui_port)
                        for gui_image, gui_port in guis_image_port
                    ]
                    + [I18N_CONF_TEMPLATE]
                )
                if gui
                else []
            )
            default_conf_template = DEFAULT_CONF_TEMPLATE.format(
                GUIS="\n\n".join(gui_conf_templates), HELPERS="\n\n".join(helpers_conf)
            )
            html_index_html_template = HTML_INDEX_HTML_TEMPLATE.format(
                IFRAMES="\n".join([ltservice.iframe for ltservice in self.ltservices if ltservice.gui]),
                SCRIPTS="\n".join([ltservice.script for ltservice in self.ltservices if ltservice.gui]),
            )

        docker_compose = DOCKER_COMPOSE.format(
            LTSERVICES="\n\n".join([ltservice.ltservice for ltservice in self.ltservices]),
            LTSERVICES_SIDECAR="\n".join(
                [ltservice.sidecarservice for ltservice in self.ltservices if ltservice.sidecarservice]
            ),
            LTSERVICES_URL="\n".join([ltservice.url for ltservice in self.ltservices]),
            EXPOSE_PORT=expose_port,
            EXPOSE_PORT_CONFIG=EXPOSE_PORT_CONFIG.format(EXPOSE_PORT=expose_port)
            if (not gui and not has_helpers)
            else "",
            GUIS="\n".join(guis) if gui else "",
            HELPERS="\n".join(helpers) if has_helpers else "",
            FRONTEND=frontend if gui or has_helpers else "",
        )
        path = Path(path)
        path.mkdir(parents=True, exist_ok=True)
        with open(path / "docker-compose.yml", "w") as f:
            f.write(docker_compose)
        if gui:
            nginx_conf_path = Path(path / "nginx-conf")
            nginx_conf_path.mkdir(parents=True, exist_ok=True)
            html_path = Path(nginx_conf_path / "html")
            html_path.mkdir(parents=True, exist_ok=True)
            with open(nginx_conf_path / "default.conf.template", "w") as f:
                f.write(default_conf_template)
            with open(html_path / "index.html.template", "w") as f:
                f.write(html_index_html_template)
            records_path = Path(nginx_conf_path / "records")
            records_path.mkdir(parents=True, exist_ok=True)
            for ltservice in self.ltservices:
                if ltservice.gui:
                    with open(records_path / f"{ltservice.name}.json", "w") as f:
                        json.dump(ltservice.record, f, default=lambda o: o.__dict__, indent=4)

        logger.info(
            "The Docker compose file has been created [{path}]. Run `docker-compose up` from the folder of Docker compose file to start the ELG REST server and the service.s.",
            path=path / "docker-compose.yml",
        )
        return
