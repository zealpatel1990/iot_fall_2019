module.exports = function(RED) {
    function c2f_node(config) {
        RED.nodes.createNode(this,config);
        var node = this;
        node.on('input', function(msg) {
            msg.payload = ((msg.payload.tempc * (9/5)) + 32).toFixed(2);
            node.send(msg);
        });
    }
    RED.nodes.registerType("C_to_F",c2f_node);
}