#!/usr/bin/env python3
import logging
import time
import pytest
from helpers.cluster import ClickHouseCluster
from multiprocessing.dummy import Pool

cluster = ClickHouseCluster(__file__)

node = cluster.add_instance(
    "node",
    main_configs=[
        "configs/async_metrics_no.xml",
    ],
    mem_limit="4g",
    env_variables={"MALLOC_CONF": "dirty_decay_ms:0"},
)


@pytest.fixture(scope="module", autouse=True)
def start_cluster():
    try:
        cluster.start()
        yield cluster
    finally:
        cluster.shutdown()


def test_multiple_queries():
    p = Pool(15)

    def run_query(node):
        try:
            node.query("SELECT * FROM system.numbers GROUP BY number")
        except Exception as ex:
            print("Exception", ex)
            raise ex

    tasks = []
    for i in range(30):
        tasks.append(p.apply_async(run_query, (node,)))
        time.sleep(i * 0.1)

    for task in tasks:
        try:
            task.get()
        except Exception as ex:
            print("Exception", ex)

    # test that we didn't kill the server
    node.query("SELECT 1")
