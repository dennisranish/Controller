var CACHE_NAME = 'site-cache-v1';
var urlsToCache = [
    '/Controller/TestWebApp/',
    '/Controller/TestWebApp/index.html',
    '/Controller/TestWebApp/manifest.json',
    '/Controller/TestWebApp/app.js',
    '/Controller/TestWebApp/giftMedium.png',
    '/Controller/TestWebApp/giftLarge.png'
];

self.addEventListener('install', function(event)
{
    // Perform install steps
    event.waitUntil(caches.open(CACHE_NAME).then(function(cache)
    {
        console.log('Opened cache');
        return cache.addAll(urlsToCache);
    }));
});

self.addEventListener('fetch', function(event)
{
    event.respondWith(caches.match(event.request).then(function(response)
    {
        // Cache hit - return response
        if (response) return response;
        return fetch(event.request);
    }));
  });