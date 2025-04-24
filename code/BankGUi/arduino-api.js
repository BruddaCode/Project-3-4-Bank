const { SerialPort } = require('serialport');

let msg = 0

const port = new SerialPort({
  path: 'COM10',
  baudRate: 9600,
});


port.on('open', function() {
    console.log('Port open')

    // leest wat de arduino te kraamen heeft
    port.on('data', (data) => {
        // bericht wordt in een string gezet omdat dat te lezen valt
        data = data.toString();

        console.log('From Arduino:', data);
        
        if(data.includes("1")) {
            msg = 0
        }
        if(data.includes("0")) {
            msg = 1
        }
    });
    
    // stuurt elke seconde een bericht naar de arduino
    // let op dat het bericht een string moet zijn, anders werkt de handel niet
    setInterval(() => {port.write(msg.toString() + "\n");}, 1000);

});