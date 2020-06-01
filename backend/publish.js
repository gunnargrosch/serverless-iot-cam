const AWS = require("aws-sdk");
const iotdata = new AWS.IotData({ endpoint: process.env.IOT_ENDPOINT });

exports.handler = async (event, context) => {
  var params = {
    topic: process.env.IOT_TOPIC_PUB,
    payload: event.body,
    qos: process.env.IOT_QOS,
  };
  const publishMessage = async (params) => {
    return new Promise((resolve, reject) => {
      iotdata.publish(params, function (err, data) {
        if (err) {
          console.log(err);
          reject(err);
        } else {
          console.log("Published message");
          resolve(params);
        }
      });
    });
  };
  return {
    statusCode: 200,
    body: JSON.stringify(await publishMessage(params)),
  };
};
