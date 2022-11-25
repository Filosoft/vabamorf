def to_camel(string: str) -> str:
    """
    Convert `snake_case` string to `lowerCamelCase`.
    """
    return "".join(word.capitalize() if i != 0 else word for i, word in enumerate(string.split("_")))
