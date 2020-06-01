## How to install Lambda function for Alexa skill

1. Open `alexaskill` subfolder in VS Code.
2. Install dependencies.
```bash
npm install
```
3. Create `env.yml` based on `env.yml.template`. Update BUCKET_NAME, IMAGE_NAME and ALEXA_SKILL_ID.
4. Deploy using Serverless Framework. Add --stage and --region if needed.
```bash
sls deploy
```
5. Test in Alexa Developer portal.