
static const char dashboardHtml[] = R"EOF(
<div id='app'>
    <h2>Rotator</h2>
    <div>Azimuth: {{info.rotator.az}}<br/>Elevation: {{info.rotator.el}}</div>
</div>
)EOF";

static const char dashboardScript[] = R"EOF(
<script>
    var app = new Vue({
        el: '#app',
        data: {
            info: null,
            timer: ''
        },
        mounted () {
            this.timer = setInterval(function() {
                axios.get('/api').then(response => (this.info = response.data))
            }.bind(this), 1000);
        }
    });
</script>
)EOF";
