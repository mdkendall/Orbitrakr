
static const char dashboardHtml[] = R"EOF(
<div id='app'>
    <div v-for="itemGroup in info">
        <h3>{{itemGroup.label}}</h3>
        <div v-for="item in itemGroup.items">
            {{item.label}} : {{(item.value/1).toFixed(item.dp)}} <span v-html="item.units"></span>
        </div>
    </div>
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
                fetch('/api').then(response => response.json()).then(data => (this.info = data));
            }.bind(this), 1000);
        }
    });
</script>
)EOF";
