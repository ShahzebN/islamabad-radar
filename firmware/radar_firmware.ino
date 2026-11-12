<!DOCTYPE html>
<html>
<head>
    <title>Islamabad Airspace Radar - Cloud</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="referrer" content="no-referrer">
    <style>
        body { background: #0d1117; color: #c9d1d9; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; padding: 20px; }
        h2 { color: #58a6ff; border-bottom: 1px solid #30363d; padding-bottom: 10px; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; background: #161b22; border-radius: 6px; overflow: hidden; }
        th, td { border: 1px solid #30363d; padding: 10px; text-align: left; font-size: 13px; vertical-align: middle; }
        th { background: #21262d; color: #58a6ff; }
        tr:nth-child(even) { background: #1c2128; }
        .badge { background: #1f6feb; color: white; padding: 3px 8px; border-radius: 12px; font-size: 11px; display: inline-block; margin-top: 4px; }
        .route { color: #8b949e; font-size: 12px; }
        .airline-logo { width: 45px; height: auto; vertical-align: middle; margin-right: 10px; border-radius: 4px; }
        #status { color: #3fb950; font-style: italic; font-size: 13px; }
        #error-box { background: #f85149; color: white; padding: 10px; border-radius: 6px; margin-bottom: 15px; display: none; }
    </style>
    <script>
        let routeCache = {};
        const targetUrl = "https://api.adsb.lol/v2/point/33.6844/73.0479/250";
        const adsbApiUrl = "https://api.allorigins.win/raw?url=" + encodeURIComponent(targetUrl);

        function fetchRadarData() {
            document.getElementById('status').innerText = 'Polling sector telemetry...';
            
            fetch(adsbApiUrl)
                .then(response => {
                    if (!response.ok) throw new Error('HTTP error! status: ' + response.status);
                    return response.json();
                })
                .then(data => {
                    // Hide any previous error banner instantly on success
                    document.getElementById('error-box').style.display = 'none';
                    let tbody = document.getElementById('radar-body');
                    let countElem = document.getElementById('ac-count');
                    tbody.innerHTML = '';
                    let count = 0;

                    if(data.ac && data.ac.length > 0) {
                        data.ac.forEach(ac => {
                            let flight = ac.flight ? ac.flight.trim() : (ac.r || 'N/A');
                            let operatorCode = (ac.ownOp || flight.substring(0, 3)).trim().toUpperCase();
                            let logoUrl = 'https://flightaware.com/images/airline_logos/90p/' + operatorCode + '.png';
                            let desc = ac.desc || ac.t || 'Commercial Airliner';
                            let alt = ac.alt_baro || ac.alt_geom || 0;
                            let speed = ac.gs || 0;
                            let lat = ac.lat ? ac.lat.toFixed(2) : '0.00';
                            let lon = ac.lon ? ac.lon.toFixed(2) : '0.00';

                            let routeDisplay = '--- &rarr; ---';
                            if (flight !== 'N/A') {
                                if (routeCache[flight]) {
                                    routeDisplay = routeCache[flight];
                                } else {
                                    routeCache[flight] = 'Fetching...';
                                    fetch('https://hexdb.io/api/v1/route/icao/' + flight)
                                        .then(r => r.json())
                                        .then(res => {
                                            if (res && res.route) {
                                                let pts = res.route.split('-');
                                                routeCache[flight] = (pts[0] || '---') + ' &rarr; ' + (pts[1] || '---');
                                            } else {
                                                routeCache[flight] = 'Unknown Route';
                                            }
                                        }).catch(() => { routeCache[flight] = '---'; });
                                }
                            }

                            let row = `<tr>
                                <td><img src='${logoUrl}' class='airline-logo' onerror="this.style.display='none'"> <strong>${flight}</strong><br><span class='badge'>${operatorCode}</span></td>
                                <td><span class='route'>${routeDisplay}</span></td>
                                <td>${desc}</td>
                                <td>${alt}</td>
                                <td>${speed}</td>
                                <td>${lat}, ${lon}</td>
                            </tr>`;
                            tbody.innerHTML += row;
                            count++;
                        });
                    } else {
                        tbody.innerHTML = '<tr><td colspan="6" style="text-align:center; color:#8b949e;">No aircraft currently reported in sector range.</td></tr>';
                    }
                    countElem.innerText = count;
                    document.getElementById('status').innerText = 'Live (Last updated: ' + new Date().toLocaleTimeString() + ')';
                })
                .catch(err => { 
                    // Silent background retry: do NOT show the red banner on minor network drops,
                    // just keep the current table active and log to console.
                    console.warn('Background poll skipped/retrying:', err);
