import sys
import psycopg2 as pg
import csv
import re

confs_csv = sys.argv[1]
journals_csv = sys.argv[2]
persons_csv = sys.argv[3]
pubs_csv = sys.argv[4]
theses_csv = sys.argv[5]

db_host = sys.argv[6]
db_port = sys.argv[7]
db_database = sys.argv[8]
db_user = sys.argv[9]
db_pw = sys.argv[10]

def checkForNonTypeElement(element):
    if element == "":
        return None
    else:
        return element

def checkForNullElement(element):
    if element == "":
        return 'NULL'
    else:
        return element

def sortConferenceRow(keyword,row_title, row_city, row_country, row_editors, row_year, row_isbn, reserved_rows):
    title_pattern1 = "Conference"
    title_pattern2 = "Fachtagung"
    title_pattern3 = "Symposium"
    title_pattern4 = "conference"
    editors_pattern = ":"
    isbn_pattern = re.compile(r'(\d\d\d)-(\d)')


    if keyword == "title":
        if title_pattern1 in row_title or title_pattern2 in row_title or title_pattern3 in row_title or title_pattern4 in row_title:
            reserved_rows.append(row_title)
            return row_title

        elif title_pattern1 in row_city or title_pattern2 in row_city or title_pattern3 in row_city or title_pattern4 in row_city:
            reserved_rows.append(row_city)
            return row_city

        elif title_pattern1 in row_country or title_pattern2 in row_country or title_pattern3 in row_country or title_pattern4 in row_country:
            reserved_rows.append(row_country)
            return row_country

        elif title_pattern1 in row_editors or title_pattern2 in row_editors or title_pattern3 in row_editors or title_pattern4 in row_editors:
            reserved_rows.append(row_editors)
            return row_editors

        elif title_pattern1 in row_year or title_pattern2 in row_year or title_pattern3 in row_year or title_pattern4 in row_year:
            reserved_rows.append(row_year)
            return row_year

        elif title_pattern1 in row_isbn or title_pattern2 in row_isbn or title_pattern3 in row_isbn or title_pattern4 in row_isbn:
            reserved_rows.append(row_isbn)
            return row_isbn

        else:
            return None

    if keyword == "country":
        if row_title in countries:
            reserved_rows.append(row_title)
            return row_title

        elif row_city in countries:
            reserved_rows.append(row_city)
            return row_city

        elif row_country in countries:
            reserved_rows.append(row_country)
            return row_country

        elif row_editors in countries:
            reserved_rows.append(row_editors)
            return row_editors

        elif row_year in countries:
            reserved_rows.append(row_year)
            return row_year

        elif row_isbn in countries:
            reserved_rows.append(row_isbn)
            return row_isbn

        else:
            return None

    if keyword == "editors":
        if editors_pattern in row_title:
            reserved_rows.append(row_title)
            return row_title

        elif editors_pattern in row_city:
            reserved_rows.append(row_city)
            return row_city

        elif editors_pattern in row_country:
            reserved_rows.append(row_country)
            return row_country

        elif editors_pattern in row_editors:
            reserved_rows.append(row_editors)
            return row_editors

        elif editors_pattern in row_year:
            reserved_rows.append(row_year)
            return row_year

        elif editors_pattern in row_isbn:
            reserved_rows.append(row_isbn)
            return row_isbn
        else:
            return None

    if keyword == "year":
        if row_title.isdigit():
            reserved_rows.append(row_title)
            return row_title

        elif row_city.isdigit():
            reserved_rows.append(row_city)
            return row_city

        elif row_country.isdigit():
            reserved_rows.append(row_country)
            return row_country

        elif row_editors.isdigit():
            reserved_rows.append(row_editors)
            return row_editors

        elif row_year.isdigit():
            reserved_rows.append(row_year)
            return row_year

        elif row_isbn.isdigit():
            reserved_rows.append(row_isbn)
            return row_isbn
        else:
            return 'NULL'

    if keyword == "isbn":
        if isbn_pattern.search(row_title):
            reserved_rows.append(row_title)
            return row_title

        elif isbn_pattern.search(row_city):
            reserved_rows.append(row_city)
            return row_city

        elif isbn_pattern.search(row_country):
            reserved_rows.append(row_country)
            return row_country

        elif isbn_pattern.search(row_editors):
            reserved_rows.append(row_editors)
            return row_editors

        elif isbn_pattern.search(row_year):
            reserved_rows.append(row_year)
            return row_year

        elif isbn_pattern.search(row_isbn):
            reserved_rows.append(row_isbn)
            return row_isbn
        else:
            return None

    if keyword == "city":
        if row_title not in reserved_rows:
            return row_title

        elif row_city not in reserved_rows:
            return row_city

        elif row_country not in reserved_rows:
            return row_country

        elif row_editors not in reserved_rows:
            return row_editors

        elif row_year not in reserved_rows:
            return row_year

        elif row_isbn not in reserved_rows:
            return row_isbn
        else:
            return None


def getYearOfHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn):

    if int(row_year) > 2000 and int(row_year) < 2021:
        return row_year

    elif int(row_type) > 2000 and int(row_type) < 2021:
        return row_type

    elif int(row_school) > 2000 and int(row_school) < 2021:
        return row_school

    elif int(row_country) > 2000 and int(row_country) < 2021:
        return row_country

    elif int(row_pages) > 2000 and int(row_pages) < 2021:
        return row_pages

    elif int(row_isbn) > 2000 and int(row_isbn) < 2021:
        return row_isbn
    else:
        return 'NULL'

def getTypeOfHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn):
    type_pattern = "PhD"

    if row_year == type_pattern:
        return row_year

    elif row_type == type_pattern:
        return row_type

    elif row_school == type_pattern:
        return row_school

    elif row_country == type_pattern:
        return row_country

    elif row_pages == type_pattern:
        return row_pages

    elif row_isbn == type_pattern:
        return row_isbn

    else:
        return None

def getPagesOfHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn):
    if row_pages.isdigit():
        return row_pages

    elif row_isbn.isdigit():
        return row_isbn
    else:
        return 'NULL'

def getIsbnOFHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn):
    isbn_pattern = re.compile(r'(\d\d\d)-(\d)')
    if isbn_pattern.search(row_pages):
        return row_pages

    elif isbn_pattern.search(row_isbn):
        return row_isbn
    else:
        return None
try:
    connection = pg.connect(user=db_user,
                            password=db_pw,
                            host=db_host,
                            port=db_port,
                            database=db_database)

    cursor = connection.cursor()

    countries = []
    pkey_authors = []
    institutions = []
    print("importing countries")
    with open(persons_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            country = row.get("Country").replace("'", "''")
            if country not in countries and country != "":
                countries.append(country)
                query = f"insert into countries(name) values('{str(country)}') on conflict do nothing"
                cursor.execute(query)

    connection.commit()
    csvfile.close()

    print("importing institutions")
    with open(persons_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            institution = row.get("Affiliation").replace("'", "''")
            country = row.get("Country").replace("'", "''")

            if institution not in institutions and institution != "" and country != "":
                institutions.append(institution)


                query = f"select cokey from countries where name = '{country}'"
                cursor.execute(query)

                cokey = cursor.fetchone()[0]

                query = f"insert into institutions(name, cokey) values('{str(institution)}', {str(cokey)}) on conflict do nothing"
                cursor.execute(query)

            if institution not in institutions and institution != "" and country == "":
                query = f"insert into institutions(name, cokey) values('{str(institution)}', NULL )on conflict do nothing"
                cursor.execute(query)

    connection.commit()
    csvfile.close()

    print("importing persons")
    with open(persons_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            akey = row.get("AKey").replace("A", "")
            name = row.get("Name").replace("'", "''")
            institution = row.get("Affiliation").replace("'", "''")
            institution = checkForNonTypeElement(institution)
            website = row.get("Website").replace("'", "''")
            website = checkForNonTypeElement(website)
            country = row.get("Country").replace("'", "''")


            if institution is not None and country is not None:
                query = f"select ikey from institutions where name = '{institution}'"
                cursor.execute(query)
                ikey = cursor.fetchone()[0]
                query = f"insert into persons(akey, name, website, ikey) values({str(akey)}, '{str(name)}', '{str(website)}',{str(ikey)}) on conflict do nothing"
                cursor.execute(query)

            else:
                query = f"insert into persons(akey, name, website, ikey) values({str(akey)}, '{str(name)}', '{str(website)}', NULL ) on conflict do nothing"
                cursor.execute(query)


    connection.commit()
    csvfile.close()

    print("importing journals")
    with open(journals_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            jkey = row.get("JKey").replace("J", "")

            name = row.get("Shortname").replace("'", "''")
            name = checkForNonTypeElement(name)

            title = row.get("Title").replace("'", "''")
            title = checkForNonTypeElement(title)

            volume = row.get("Volume").replace("'", "''")
            volume = checkForNullElement(volume)

            issue = row.get("Number").replace("'", "''")
            issue = checkForNullElement(issue)

            year = row.get("Year").replace("'", "''")
            year = checkForNullElement(year)

            query = f"insert into journals(jkey, sname, title, volume, issue, year) values({str(jkey)}, '{str(name)}', '{str(title)}',{str(volume)}, {str(issue)}, {str(year)}) on conflict do nothing"
            cursor.execute(query)

    connection.commit()
    csvfile.close()

    print("importing conferences")
    with open(confs_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            ckey = row.get("CKey").replace("C", "")

            sname = row.get("Shortname").replace("'", "''")


            row_title = row.get("Title").replace("'", "''")


            row_city = row.get("City").replace("'", "''")

            row_country = row.get("Country").replace("'", "''")

            row_editors = row.get("Editors").replace("'", "''")

            row_year = row.get("Year").replace("'", "''")

            row_isbn = row.get("ISBN").replace("'", "''")

            #there is no real search pattern for city so i choos for city the row which wasnt used yet
            reserved_rows = []

            title = "title"
            title = sortConferenceRow(title,row_title,row_city, row_country, row_editors, row_year, row_isbn, reserved_rows)

            country = "country"
            country = sortConferenceRow(country,row_title,row_city, row_country, row_editors, row_year, row_isbn, reserved_rows)

            editors = "editors"
            editors = sortConferenceRow(editors,row_title,row_city, row_country, row_editors, row_year, row_isbn, reserved_rows)

            year = "year"
            year = sortConferenceRow(year,row_title,row_city, row_country, row_editors, row_year, row_isbn, reserved_rows)

            isbn = "isbn"
            isbn = sortConferenceRow(isbn,row_title,row_city, row_country, row_editors, row_year, row_isbn, reserved_rows)

            city = "city"
            city = sortConferenceRow(city,row_title,row_city, row_country, row_editors, row_year, row_isbn, reserved_rows)


            if country in countries:

                query = f"select cokey from countries where name = '{country}'"
                cursor.execute(query)
                cokey = cursor.fetchone()[0]
                query = f"insert into conferences(ckey, sname, title, city, cokey, year, isbn) values({str(ckey)}, '{str(sname)}', '{str(title)}','{str(city)}', {str(cokey)}, {str(year)},  '{str(isbn)}') on conflict do nothing"
                cursor.execute(query)

            else:
                query = f"insert into conferences(ckey, sname, title, city, cokey, year, isbn) values({str(ckey)}, '{str(sname)}', '{str(title)}','{str(city)}', NULL, {str(year)},  '{str(isbn)}') on conflict do nothing"
                cursor.execute(query)

    connection.commit()
    csvfile.close()

    print("importing papers")
    with open(pubs_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:

            pkey = row.get("PKey").replace("P", "")

            row_title = row.get("Title").replace("'", "''")

            row_authors = row.get("Authors").replace("'", "''")

            row_pages = row.get("Pages").replace("'", "''")

            row_cjkey = row.get("CJKey").replace("'", "")

            line_pkey_authors = [pkey, row_authors]



            pkey_authors.append(line_pkey_authors)



            ckey = 'NULL'
            jkey = 'NULL'
            if 'J' in row_cjkey:
                jkey = row_cjkey.replace("J", "")
                query = f"insert into papers(pkey, title, pages, ckey, jkey) values({str(pkey)}, '{str(row_title)}','{str(row_pages)}', {str(ckey)}, {str(jkey)}) on conflict do nothing"
                cursor.execute(query)

            if 'C' in row_cjkey:
                ckey = row_cjkey.replace("C", "")
                query = f"insert into papers(pkey, title, pages, ckey, jkey) values({str(pkey)}, '{str(row_title)}','{str(row_pages)}', {str(ckey)}, {str(jkey)}) on conflict do nothing"
                cursor.execute(query)



    connection.commit()
    csvfile.close()
    '''''''''
    print("importing theses")
    with open(theses_csv) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            tkey = row.get("TKey").replace("T", "")
            row_author = row.get("Author").replace("'", "''")
            row_title = row.get("Title").replace("'", "''")

            row_year = row.get("Year").replace("'", "''")
            row_type = row.get("Type").replace("'", "''")
            row_school = row.get("School").replace("'", "''")
            row_country = row.get("Country").replace("'", "''")
            row_pages = row.get("Pages").replace("'", "''")
            row_isbn = row.get("ISBN").replace("'", "''")


            year = getYearOfHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn)
            type = getTypeOfHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn)
            pages = getPagesOfHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn)
            isbn = getIsbnOFHoleRow(row_year, row_type, row_school, row_country, row_pages, row_isbn)

            akey = 'NULL'
            ikey = 'NULL'


    '''''''''













except (Exception, pg.Error) as error:
    print("Error:", error)

finally:
    if connection:
        cursor.close()
        connection.close()