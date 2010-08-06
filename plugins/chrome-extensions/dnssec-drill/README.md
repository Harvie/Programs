Simple DNSSEC validator plugin for chromium, but it's in early stage of development but it works somehow. Only thing it does is showing red icon on fraudent sites and yellow on every other site (trusted and non-signed). it uses external binary of program called drill (from ldns or libldns package), which is launched with npexec plugin (NPAPI which enables you to call system() call from javascript) right now it's synchronous and therefore very slow. i want to implement following things in future (if i will found it usefull):

TODO:
* multiple DNSSEC lookups at the time (asynchronous or multithreaded NPAPI plugin).
* block user from using sites with fraudent DNS records (not only showing icons)
* distinguish between non-signed and secure domains (parse drill output or patch drill)
* compile NPAPI plugin for Windows, Linux64 and MacOS. Currently i have only .so for 32bit Linux, but it should build as it is without any code changes.
* pack static drill binary and root-zone key to plugin
* plugin configuration page (change drill binary path or root-zone certificate, whitelist domains)
