const express = require('express')
const app = express()
var cors = require('cors')
const port = 3000

app.use(cors())
app.use(express.json())

let temp = 0;
let hum = 0;

app.get('/', (req, res) => {
  res.send('Hello World!')
})

app.post('/data', (req, res) => {
    // console.log("Nhan du lieu:", req.body);
    temp = req.body.temp;
    hum = req.body.hum;
    console.log("Temperature:", temp);
    console.log("Humidity:", hum);
    res.json(1);
});

app.get("/data", (req, res) => {
  let mData = { mTem: temp, mHum: hum };
  res.json(mData);
});


app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
