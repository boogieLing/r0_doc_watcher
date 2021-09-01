def query_generator(
        gte: int = None, lte: int = 0, time_unit: str = "d",
        page: int = None, size: int = 10, search_after: list = None,
        time_field: str = "@timestamp", include: str = None, exclude: str = None, filters: list = None
):
    """
    根据时间和页面大小生成查询query

    如果指定了search_after, 或者只指定了size没有page，那么分页查询基于search after，核心是利用排序游标，在这里使用时间戳作为降序游标，以_id作为可选升序游标

    如果指定了page和size，那么分页查询基于from-size

    :param gte: 起始时间到现在的距离
    :param lte: 终止时间到现在的距离
    :param time_unit: 时间单位，默认为d(day)
    :param page: 页面序号
    :param size: 页面大小，但此选项在_delete_by_query会被忽略
    :param search_after: 上一次查询的最后一个游标
    :param time_field: 时间戳字段名称
    :param include: 输出时保留的字段
    :param exclude: 输出时忽略的字段
    :param filters: 过滤条件
    :return: 查询用的query
    """

    ans_query = {
        "size": size,
        "query": {
            "bool": {
                "filter": [
                    {
                        "range": {
                            time_field: {
                                "lte": f"now-{lte}{time_unit}",
                                "format": "epoch_millis"
                            }
                        }
                    },
                ],
            }
        },
        "_source": {

        }
    }
    if page and search_after:
        return {}
    if page is None:
        ans_query.update({
            "sort": [
                {
                    time_field: "desc",
                    "_id": "asc"
                }
            ]
        })
    if search_after is not None:
        ans_query.update({
            "search_after": search_after
        })
    if page is not None:
        ans_query.update({
            "from": page * size
        })
    if gte is not None:
        ans_query["query"]["bool"]["filter"][0]["range"][time_field].update({
            "gte": f"now-{gte}{time_unit}",
        })
    if include is not None and exclude is not None:
        raise Exception("Cannot specify both 'fields' and 'ignore_fields'")
    if include is not None:
        include = ih.get_list_from_arg_strings(include)
        ans_query["_source"].update({
            "include": include
        })
    if exclude is not None:
        exclude = ih.get_list_from_arg_strings(exclude)
        ans_query["_source"].update({
            "exclude": exclude
        })
    if filters is not None:
        ans_query["query"]["bool"]["filter"].extend(filters)
    return ans_query
