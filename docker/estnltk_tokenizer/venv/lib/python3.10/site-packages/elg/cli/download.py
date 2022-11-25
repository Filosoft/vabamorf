import sys
from argparse import ArgumentParser

from loguru import logger

from . import BaseELGCommand


def download_command_factory(args):
    return DownloadCommand(
        id=args.id,
        domain=args.domain,
        not_use_cache=args.not_use_cache,
        cache_dir=args.cache_dir,
        distribution_idx=args.distribution_idx,
        filename=args.filename,
        folder=args.folder,
    )


class DownloadCommand(BaseELGCommand):
    @staticmethod
    def register_subcommand(parser: ArgumentParser):
        download_parser = parser.add_parser("download", description="Download an ELG corpus")
        download_parser.add_argument(
            "--id", type=int, default=None, required=True, help="ID of the corpus to download"
        )
        download_parser.add_argument(
            "--domain",
            type=str,
            default="live",
            required=False,
            help="ELG domain you want to use",
        )
        download_parser.add_argument(
            "--not_use_cache",
            default=False,
            action="store_true",
            required=False,
            help="use to not use cache",
        )
        download_parser.add_argument(
            "--cache_dir", type=str, default="~/.cache/elg", required=False, help="path to the cache directory"
        )
        download_parser.add_argument(
            "--distribution_idx",
            type=int,
            default=0,
            required=False,
            help="index of the distribution of the corpus to download",
        )
        download_parser.add_argument(
            "--filename",
            default=None,
            type=str,
            required=False,
            help="name of the output file. If None, the name of the corpus will be used",
        )
        download_parser.add_argument(
            "--folder",
            type=str,
            default="./",
            required=False,
            help="path to the folder where to save the downloaded file",
        )
        download_parser.set_defaults(func=download_command_factory)

    def __init__(
        self,
        id: int = None,
        domain: str = "live",
        not_use_cache: bool = False,
        cache_dir: str = "~/.cache/elg",
        distribution_idx: int = 0,
        filename: str = None,
        folder: str = "./",
    ):
        self._id = id
        self._domain = domain
        self._use_cache = not not_use_cache
        self._cache_dir = cache_dir
        self._distribution_idx = distribution_idx
        self._filename = filename
        self._folder = folder

    def run(self):
        from ..corpus import Corpus

        try:
            corpus = Corpus.from_id(
                id=self._id,
                domain=self._domain,
                use_cache=self._use_cache,
                cache_dir=self._cache_dir,
            )
            logger.info(f"Downloading {corpus.resource_name}")
            corpus.download(
                distribution_idx=self._distribution_idx,
                filename=self._filename,
                folder=self._folder,
            )
        except Exception as e:
            logger.error(f"Error during corpus download - {e}")
            sys.exit(1)
