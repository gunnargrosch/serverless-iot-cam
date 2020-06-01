# Serverless IoT Cam

**Sample code for using ESP32-CAM with AWS IoT Core to take pictures on-demand and store them in S3.**

## About

Using the sample code you can connect an ESP32-CAM device to AWS IoT Core and have it take pictures on-demand and store them in S3. Uses serverless technology and fully managed services.

 ![Serverless IoT Cam Architecture](serverless-iot-cam-architecture.png?raw=true "Serverless IoT Cam Architecture")

## How to setup device

1. Open `device` subfolder in VS Code and install PlatformIO extension.
2. Create your device using the `Creating AWS IoT things` wizard in AWS Console (or using cli for power users). Take note of the thing name and download the certificates and CA certificate.
3. Create a policy using the AWS Console or cli. See `policy.json.template` for needed policies.
4. Create `lib/conf/conf.h` based on `lib/conf/conf.h.template`. Update WIFI_SSID, WIFI_PASSWORD, DEVICE_NAME, AWS_IOT_SUB_TOPIC, AWS_IOT_PUB_TOPIC, AWS_IOT_ENDPOINT and the three certificates. 
5. Flash your device. Keep an eye on the serial output to see that all goes well.

## How to setup backend using Serverless Framework

1. Open `backend` subfolder in VS Code.
2. Create `env.yml` based on `env.yml.template`. Update at minimum IOT_ENDPOINT, IOT_TOPIC_PUB (should be the device's sub topic) and IOT_TOPIC_SUB (should be the device's pub topic).
3. Deploy using Serverless Framework. Add --stage and --region if needed.
```bash
sls deploy
```
4. Test the flow by posting to the endpoint shown in output.

## Questions

Reach out to [Gunnar Grosch](https://twitter.com/gunnargrosch) if you have any questions.

## Author

**Gunnar Grosch** - [GitHub](https://github.com/gunnargrosch) | [Twitter](https://twitter.com/gunnargrosch) | [LinkedIn](https://www.linkedin.com/in/gunnargrosch/)