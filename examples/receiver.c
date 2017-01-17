0 info it worked if it ends with ok
1 verbose cli [ '/usr/local/bin/node',
1 verbose cli   '/usr/local/bin/npm',
1 verbose cli   'install',
1 verbose cli   '-g',
1 verbose cli   'socketcluster' ]
2 info using npm@2.10.1
3 info using node@v0.12.4
4 verbose install initial load of /usr/local/lib/package.json
5 verbose readDependencies loading dependencies from /usr/local/lib/package.json
6 silly cache add args [ 'socketcluster', null ]
7 verbose cache add spec socketcluster
8 silly cache add parsed spec { raw: 'socketcluster',
8 silly cache add   scope: null,
8 silly cache add   name: 'socketcluster',
8 silly cache add   rawSpec: '',
8 silly cache add   spec: '*',
8 silly cache add   type: 'range' }
9 silly addNamed socketcluster@*
10 verbose addNamed "*" is a valid semver range for socketcluster
11 silly addNameRange { name: 'socketcluster', range: '*', hasData: false }
12 silly mapToRegistry name socketcluster
13 silly mapToRegistry using default registry
14 silly mapToRegistry registry https://registry.npmjs.org/
15 silly mapToRegistry uri https://registry.npmjs.org/socketcluster
16 verbose addNameRange registry:https://registry.npmjs.org/socketcluster not in flight; fetching
17 verbose request uri https://registry.npmjs.org/socketcluster
18 verbose request no auth needed
19 info attempt registry request try #1 at 2:36:41 PM
20 verbose request id b5000f57b28cfc0d
21 verbose etag "1EZPF8MMUA69QAMV0BKGNVB8O"
22 http request GET https://registry.npmjs.org/socketcluster
23 http 304 https://registry.npmjs.org/socketcluster
24 silly get cb [ 304,
24 silly get   { date: 'Wed, 13 Jul 2016 09:06:41 GMT',
24 silly get     via: '1.1 varnish',
24 silly get     'cache-control': 'max-age=300',
24 silly get     etag: '"1EZPF8MMUA69QAMV0BKGNVB8O"',
24 silly get     age: '0',
24 silly get     connection: 'keep-alive',
24 silly get     'x-served-by': 'cache-sin6920-SIN',
24 silly get     'x-cache': 'HIT',
24 silly get     'x-cache-hits': '1',
24 silly get     'x-timer': 'S1468400801.749000,VS0,VE209',
24 silly get     vary: 'Accept-Encoding' } ]
25 verbose etag https://registry.npmjs.org/socketcluster from cache
26 verbose get saving socketcluster to /home/sachin/.npm/registry.npmjs.org/socketcluster/.cache.json
27 silly addNameRange number 2 { name: 'socketcluster', range: '*', hasData: true }
28 silly addNameRange versions [ 'socketcluster',
28 silly addNameRange   [ '0.9.0',
28 silly addNameRange     '0.9.1',
28 silly addNameRange     '0.9.2',
28 silly addNameRange     '0.9.3',
28 silly addNameRange     '0.9.4',
28 silly addNameRange     '0.9.5',
28 silly addNameRange     '0.9.6',
28 silly addNameRange     '0.9.7',
28 silly addNameRange     '0.9.8',
28 silly addNameRange     '0.9.10',
28 silly addNameRange     '0.9.11',
28 silly addNameRange     '0.9.12',
28 silly addNameRange     '0.9.13',
28 silly addNameRange     '0.9.14',
28 silly addNameRange     '0.9.15',
28 silly addNameRange     '0.9.16',
28 silly addNameRange     '0.9.17',
28 silly addNameRange     '0.9.18',
28 silly addNameRange     '0.9.19',
28 silly addNameRange     '0.9.20',
28 silly addNameRange     '0.9.21',
28 silly addNameRange     '0.9.22',
28 silly addNameRange     '0.9.23',
28 silly addNameRange     '0.9.24',
28 silly addNameRange     '0.9.25',
28 silly addNameRange     '0.9.26',
28 silly addNameRange     '0.9.27',
28 silly addNameRange     '0.9.28',
28 silly addNameRange     '0.9.29',
28 silly addNameRange     '0.9.30',
28 silly addNameRange     '0.9.31',
28 silly addNameRange     '0.9.32',
28 silly addNameRange     '0.9.33',
28 silly addNameRange     '0.9.34',
28 silly addNameRange     '0.9.35',
28 silly addNameRange     '0.9.36',
28 silly addNameRange     '0.9.37',
28 silly addNameRange     '0.9.38',
28 silly addNameRange     '0.9.39',
28 silly addNameRange     '0.9.41',
28 silly addNameRange     '0.9.42',
28 silly addNameRange     '0.9.43',
28 silly addNameRange     '0.9.44',
28 silly addNameRange     '0.9.45',
28 silly addNameRange     '0.9.46',
28 silly addNameRange     '0.9.47',
28 silly addNameRange     '0.9.48',
28 silly addNameRange     '0.9.50',
28 silly addNameRange     '0.9.51',
28 silly addNameRange     '0.9.52',
28 silly addNameRange     '0.9.53-pre.1',
28 silly addNameRange     '0.9.53-pre.2',
28 silly addNameRange     '0.9.53',
28 silly addNameRange     '0.9.54',
28 silly addNameRange     '0.9.55',
28 silly addNameRange     '0.9.56',
28 silly addNameRange     '0.9.57',
28 silly addNameRange     '0.9.58',
28 silly addNameRange     '0.9.59',
28 silly addNameRange     '0.9.60',
28 silly addNameRange     '0.9.61',
28 silly addNameRange     '0.9.62',
28 silly addNameRange     '0.9.63',
28 silly addNameRange     '0.9.64',
28 silly addNameRange     '0.9.65',
28 silly addNameRange     '0.9.66',
28 silly addNameRange     '0.9.67',
28 silly addNameRange     '0.9.68',
28 silly addNameRange     '0.9.69',
28 silly addNameRange     '0.9.70',
28 silly addNameRange     '0.9.71',
28 silly addNameRange     '0.9.72',
28 silly addNameRange     '0.9.73',
28 silly addNameRange     '0.9.74',
28 silly addNameRange     '0.9.75',
28 silly addNameRange     '0.9.76',
28 silly addNameRange     '0.9.77',
28 silly addNameRange     '0.9.78',
28 silly addNameRange     '0.9.79',
28 silly addNameRange     '0.9.80',
28 silly addNameRange     '0.9.81',
28 silly addNameRange     '0.9.82',
28 silly addNameRange     '0.9.83',
28 silly addNameRange     '0.9.84',
28 silly addNameRange     '0.9.85',
28 silly addNameRange     '0.9.87',
28 silly addNameRange     '0.9.88',
28 silly addNameRange     '0.9.89',
28 silly addNameRange     '0.9.90',
28 silly addNameRange     '0.9.91',
28 silly addNameRange     '1.0.1',
28 silly addNameRange     '1.0.2',
28 silly addNameRange     '1.0.3',
28 silly addNameRange     '1.0.4',
28 silly addNameRange     '1.0.5',
28 silly addNameRange     '1.0.6',
28 silly addNameRange     '1.0.7',
28 silly addNameRange     '1.0.8',
28 silly addNameRange     '1.0.9',
28 silly addNameRange     '1.1.0',
28 silly addNameRange     '1.1.1',
28 silly addNameRange     '1.1.2',
28 silly addNameRange     '1.1.3',
28 silly addNameRange     '1.1.4',
28 silly addNameRange     '1.1.5',
28 silly addNameRange     '1.1.6',
28 silly addNameRange     '1.1.7',
28 silly addNameRange     '1.1.8',
28 silly addNameRange     '1.1.9',
28 silly addNameRange     '1.2.0',
28 silly addNameRange     '1.2.1',
28 silly addNameRange     '1.2.2',
28 silly addNameRange     '1.2.3',
28 silly addNameRange     '1.2.4',
28 silly addNameRange     '1.2.5',
28 silly addNameRange     '1.3.0',
28 silly addNameRange     '1.3.1',
28 silly addNameRange     '1.3.2',
28 silly addNameRange     '1.3.3',
28 silly addNameRange     '1.3.4',
28 silly addNameRange     '1.3.5',
28 silly addNameRange     '1.3.6',
28 silly addNameRange     '1.3.7',
28 silly addNameRange     '1.3.8',
28 silly addNameRange     '1.3.9',
28 silly addNameRange     '1.3.10',
28 silly addNameRange     '1.3.11',
28 silly addNameRange     '2.2.9',
28 silly addNameRange     '2.2.10',
28 silly addNameRange     '2.2.11',
28 silly addNameRange     '2.2.12',
28 silly addNameRange     '2.2.13',
28 silly addNameRange     '2.2.14',
28 silly addNameRange     '2.2.15',
28 silly addNameRange     '2.2.16',
28 silly addNameRange     '2.2.17',
28 silly addNameRange     '2.2.18',
28 silly addNameRange     '2.2.19',
28 silly addNameRange     '2.2.20',
28 silly addNameRange     '2.2.21',
28 silly addNameRange     '2.2.22',
28 silly addNameRange     '2.2.23',
28 silly addNameRange     '2.2.24',
28 silly addNameRange     '2.2.25',
28 silly addNameRange     '2.2.26',
28 silly addNameRange     '2.2.30',
28 silly addNameRange     '2.2.31',
28 silly addNameRange     '2.2.32',
28 silly addNameRange     '2.2.33',
28 silly addNameRange     '2.2.34',
28 silly addNameRange     '2.2.35',
28 silly addNameRange     '2.2.36',
28 silly addNameRange     '2.2.37',
28 silly addNameRange     '2.2.38',
28 silly addNameRange     '2.2.39',
28 silly addNameRange     '2.3.0',
28 silly addNameRange     '2.3.1',
28 silly addNameRange     '2.3.3',
28 silly addNameRange     '2.3.4',
28 silly addNameRange     '2.3.5',
28 silly addNameRange     '2.3.6',
28 silly addNameRange     '2.3.7',
28 silly addNameRange     '2.3.8',
28 silly addNameRange     '2.3.9',
28 silly addNameRange     '2.3.10',
28 silly addNameRange     '2.3.11',
28 silly addNameRange     '2.3.12',
28 silly addNameRange     '2.3.13',
28 silly addNameRange     '2.3.14',
28 silly addNameRange     '2.3.15',
28 silly addNameRange     '2.3.16',
28 silly addNameRange     '2.3.17',
28 silly addNameRange     '2.3.18',
28 silly addNameRange     '2.3.19',
28 silly addNameRange     '2.3.20',
28 silly addNameRange     '2.3.21',
28 silly addNameRange     '2.3.22',
28 silly addNameRange     '2.3.23',
28 silly addNameRange     '2.3.24',
28 silly addNameRange     '2.3.25',
28 silly addNameRange     '2.3.26',
28 silly addNameRange     '2.4.0',
28 silly addNameRange     '2.5.0',
28 silly addNameRange     '2.5.1',
28 silly addNameRange     '2.5.2',
28 silly addNameRange     '2.5.3',
28 silly addNameRange     '3.0.0',
28 silly addNameRange     '4.0.0',
28 silly addNameRange     '4.0.1',
28 silly addNameRange     '4.1.0',
28 silly addNameRange     '4.2.0',
28 silly addNameRange     '4.2.1',
28 silly addNameRange     '4.2.2',
28 silly addNameRange     '4.3.0',
28 silly addNameRange     '4.3.1',
28 silly addNameRange     '4.3.2',
28 silly addNameRange     '4.3.3',
28 silly addNameRange     '4.3.4',
28 silly addNameRange     '4.3.5',
28 silly addNameRange     '4.3.6',
28 silly addNameRange     '4.3.7',
28 silly addNameRange     '4.4.0',
28 silly addNameRange     '4.5.0',
28 silly addNameRange     '4.6.0',
28 silly addNameRange     '4.6.1',
28 silly addNameRange     '4.6.2' ] ]
29 silly addNamed socketcluster@4.6.2
30 verbose addNamed "4.6.2" is a plain semver version for socketcluster
31 silly cache afterAdd socketcluster@4.6.2
32 verbose afterAdd /home/sachin/.npm/socketcluster/4.6.2/package/package.json not in flight; writing
33 verbose afterAdd /home/sachin/.npm/socketcluster/4.6.2/package/package.json written
34 silly install resolved [ { name: 'socketcluster',
34 silly install resolved     description: 'SocketCluster - A Highly parallelized WebSocket server cluster to make the most of multi-core machines/instances.',
34 silly install resolved     version: '4.6.2',
34 silly install resolved     homepage: 'http://socketcluster.io',
34 silly install resolved     contributors: [ [Object] ],
34 silly install resolved     repository:
34 silly install resolved      { type: 'git',
34 silly install resolved        url: 'git://github.com/SocketCluster/socketcluster.git' },
34 silly install resolved     dependencies:
34 silly install resolved      { async: '1.5.0',
34 silly install resolved        base64id: '0.1.0',
34 silly install resolved        'fs-extra': '0.30.0',
34 silly install resolved        jsonwebtoken: '5.4.1',
34 silly install resolved        minimist: '1.1.0',
34 silly install resolved        'node-uuid': '1.4.3',
34 silly install resolved        'sc-auth': '3.x.x',
34 silly install resolved        'sc-broker-cluster': '2.x.x',
34 silly install resolved        'sc-emitter': '1.x.x',
34 silly install resolved        'sc-errors': '1.x.x',
34 silly install resolved        'sc-formatter': '1.x.x',
34 silly install resolved        'socketcluster-server': '4.x.x',
34 silly install resolved        'uid-number': '0.0.5' },
34 silly install resolved     bin: { socketcluster: 'bin/cli.js' },
34 silly install resolved     keywords: [ 'websocket', 'server', 'realtime', 'cluster', 'scalable' ],
34 silly install resolved     license: 'MIT',
34 silly install resolved     gitHead: 'b2985a42a80c6f257ec0c9c338d3f52498c26dfe',
34 silly install resolved     bugs: { url: 'https://github.com/SocketCluster/socketcluster/issues' },
34 silly install resolved     _id: 'socketcluster@4.6.2',
34 silly install resolved     scripts: {},
34 silly install resolved     _shasum: 'd09eb3eda268b771c6eede44252eefd29a6a2e3c',
34 silly install resolved     _from: 'socketcluster@*',
34 silly install resolved     _npmVersion: '3.8.6',
34 silly install resolved     _nodeVersion: '6.1.0',
34 silly install resolved     _npmUser: { name: 'topcloudsystems', email: 'grosjona@yahoo.com.au' },
34 silly install resolved     dist:
34 silly install resolved      { shasum: 'd09eb3eda268b771c6eede44252eefd29a6a2e3c',
34 silly install resolved        tarball: 'https://registry.npmjs.org/socketcluster/-/socketcluster-4.6.2.tgz' },
34 silly install resolved     maintainers: [ [Object] ],
34 silly install resolved     _npmOperationalInternal:
34 silly install resolved      { host: 'packages-16-east.internal.npmjs.com',
34 silly install resolved        tmp: 'tmp/socketcluster-4.6.2.tgz_1464079466685_0.7951844020280987' },
34 silly install resolved     directories: {},
34 silly install resolved     _resolved: 'https://registry.npmjs.org/socketcluster/-/socketcluster-4.6.2.tgz',
34 silly install resolved     readme: 'ERROR: No README data found!' } ]
35 info install socketcluster@4.6.2 into /usr/local/lib
36 info installOne socketcluster@4.6.2
37 verbose installOne of socketcluster to /usr/local/lib not in flight; installing
38 verbose lock using /home/sachin/.npm/_locks/socketcluster-e2539cdce564f0b5.lock for /usr/local/lib/node_modules/socketcluster
39 silly install write writing socketcluster 4.6.2 to /usr/local/lib/node_modules/socketcluster
40 verbose unbuild lib/node_modules/socketcluster
41 info preuninstall socketcluster@4.6.2
42 info uninstall socketcluster@4.6.2
43 verbose unbuild rmStuff socketcluster@4.6.2 from /usr/local/lib/node_modules
44 silly gentlyRm /usr/local/bin/socketcluster is being gently removed
45 silly gentlyRm verifying /usr/local is an npm working directory
46 silly gentlyRm containing path /usr/local is under npm's control, in /usr/local
47 silly gentlyRm deletion target /usr/local/bin/socketcluster is under /usr/local
48 verbose gentlyRm vacuuming from /usr/local/bin/socketcluster up to /usr/local
49 silly vacuum-fs removing /usr/local/bin/socketcluster
50 verbose unlock done using /home/sachin/.npm/_locks/socketcluster-e2539cdce564f0b5.lock for /usr/local/lib/node_modules/socketcluster
51 verbose stack Error: EACCES, unlink '/usr/local/bin/socketcluster'
51 verbose stack     at Error (native)
52 verbose cwd /home/sachin/Templates
53 error Linux 3.13.0-32-generic
54 error argv "/usr/local/bin/node" "/usr/local/bin/npm" "install" "-g" "socketcluster"
55 error node v0.12.4
56 error npm  v2.10.1
57 error path /usr/local/bin/socketcluster
58 error code EACCES
59 error errno -13
60 error Error: EACCES, unlink '/usr/local/bin/socketcluster'
60 error     at Error (native)
60 error  { [Error: EACCES, unlink '/usr/local/bin/socketcluster']
60 error   errno: -13,
60 error   code: 'EACCES',
60 error   path: '/usr/local/bin/socketcluster' }
61 error Please try running this command again as root/Administrator.
62 verbose exit [ -13, true ]
63 verbose unbuild lib/node_modules/socketcluster
64 info preuninstall socketcluster@4.6.2
65 info uninstall socketcluster@4.6.2
66 verbose unbuild rmStuff socketcluster@4.6.2 from /usr/local/lib/node_modules
67 silly gentlyRm /usr/local/bin/socketcluster is being gently removed
68 silly gentlyRm verifying /usr/local is an npm working directory
69 silly gentlyRm containing path /usr/local is under npm's control, in /usr/local
70 silly gentlyRm deletion target /usr/local/bin/socketcluster is under /usr/local
71 verbose gentlyRm vacuuming from /usr/local/bin/socketcluster up to /usr/local
72 silly vacuum-fs removing /usr/local/bin/socketcluster
73 error error rolling back Error: EACCES, unlink '/usr/local/bin/socketcluster'
73 error error rolling back     at Error (native)
73 error error rolling back  { [Error: EACCES, unlink '/usr/local/bin/socketcluster']
73 error error rolling back   errno: -13,
73 error error rolling back   code: 'EACCES',
73 error error rolling back   path: '/usr/local/bin/socketcluster' }
