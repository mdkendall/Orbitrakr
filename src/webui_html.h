
static const char dashboardHtml[] = R"EOF(
<div id='app'>
    <div v-for="itemGroup in info">
        <h3>{{itemGroup.label}}</h3>
        <div v-for="item in itemGroup.items">
            {{item.label}} : {{(item.value/1).toFixed(1)}} <span v-html="item.units"></span>
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
                axios.get('/api').then(response => (this.info = response.data))
            }.bind(this), 1000);
        }
    });
</script>
)EOF";
