
static const char dashboardHtml[] = R"EOF(
<div id='app'>
    <div>
        <svg xmlns="http://www.w3.org/2000/svg" viewbox="-110 -110 220 220">
            <circle cx="0" cy="0" r="100" stroke="gray" stroke-width="1" fill="none"/>
            <circle cx="0" cy="0" r="66" stroke="gray" stroke-width="1" fill="none"/>
            <circle cx="0" cy="0" r="33" stroke="gray" stroke-width="1" fill="none"/>
            <line x1="0" y1="-105" x2="0" y2="105" stroke="gray" stroke-width="1"/>
            <line x1="-105" y1="0" x2="105" y2="0" stroke="gray" stroke-width="1"/>
            <circle :cx="plot.rx" :cy="plot.ry" r="8" stroke="black" stroke-width="2" fill="#ACC2A3"/>
            <circle :cx="plot.tx" :cy="plot.ty" r="5" stroke="black" stroke-width="2" fill="#5E72A7"/>
        </svg>
    </div>
    <div v-for="(itemGroup, itemGroupId) in info">
        <h3>{{itemGroup.label}}</h3>
        <div>
            <div v-for="(item, itemId) in itemGroup.items">
                {{item.label}} : {{(item.value/1).toFixed(item.dp)}} <span v-html="item.units"></span>
                <template v-if="item.requestable"><input class="dash" v-bind:ref="itemGroupId+'.'+itemId"/> <button class="dash" v-on:click="putItem(itemGroupId, itemId, $refs[itemGroupId+'.'+itemId][0].value)">Set</button></template>
            </div>
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
            plot: {"rx":0, "ry":0, "tx":0, "ty":0},
            timer: ''
        },
        methods: {
            updatePlot () {
                this.plot.rx = 100*(1-this.info.rotator.items.el.value/90)*Math.sin(Math.PI*this.info.rotator.items.az.value/180);
                this.plot.ry = -100*(1-this.info.rotator.items.el.value/90)*Math.cos(Math.PI*this.info.rotator.items.az.value/180);
                this.plot.tx = 100*(1-this.info.tracker.items.el.value/90)*Math.sin(Math.PI*this.info.tracker.items.az.value/180);
                this.plot.ty = -100*(1-this.info.tracker.items.el.value/90)*Math.cos(Math.PI*this.info.tracker.items.az.value/180);
            },
            putItem(itemGroupId, itemId, value) {
                setData = JSON.stringify({value: value});
                fetch(`/api/${itemGroupId}/items/${itemId}`, {method: 'PUT', headers: {'Content-Type': 'application/json'}, body: setData});
            },
        },
        mounted () {
            this.timer = setInterval(function() {
                fetch('/api').then(response => response.json()).then(data => {this.info = data; this.updatePlot();});
            }.bind(this), 1000);
        },
    });
</script>
)EOF";
