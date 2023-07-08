#!/bin/bash

pip3 install psycopg2

python3 IngestData.py ./confs.csv ./journals.csv ./persons.csv ./pubs.csv ./theses.csv \localhost 5432 db01556194 postgres tischtennis