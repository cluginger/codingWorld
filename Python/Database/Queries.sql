--Q01:Where did the conference SIGMOD 2019 (short name, year) take place?
select city
from conferences
where sname = 'SIGMOD' and year = 2019

--Q02:Which persons are affiliated with Graz University of Technology? (return name, website; sorted ascending by name)
select persons.name, persons.website
from persons, institutions
where institutions.ikey = persons.ikey and institutions.name = 'Graz University of Technology'
order by name asc

--Q04: Which journal issues contained more than 70 papers?(return title, volume, issue, year; sorted descending by year and issue)
select title, volume, issue, year
from journals
where volume > 70
order by year desc, issue desc

--Q05:Which cities hosted more than 2 conferences? (return city, country, count; sorted decreasing by count)
select conferences.city, countries.name, count(conferences.city)
from conferences, countries
where countries.cokey = conferences.cokey
group by conferences.city, countries.name
having count(conferences.city) > 2
order by count(conferences.city) desc
