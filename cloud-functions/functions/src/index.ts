import * as functions from 'firebase-functions';
import * as jwt from 'jsonwebtoken';
import * as firebaseAdmin from 'firebase-admin';
import * as cors from 'cors';

const corsHandler = cors({origin: true});

const publicKey = "-----BEGIN PUBLIC KEY-----\n" +
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArOcF2IQ5tN1XiV+4Ut0L\n" +
    "xKbZA66s0sHpB/70kaNXnP2l/Pkwxu39cYHqylZC5uiqZE8piHmGyZMeAlLF1L/X\n" +
    "zX3J+ye+JOGeH0GJyJQ4YWHpIjvPdm9VuXjUzrZQG0Sx3vI4fQ8dfaZdgKLHamLN\n" +
    "WN+Xh9ptJo7Z1InOemeTx5fothT+I9JV1fWMx6G9PbC59dXJXeF2HPWpWxulRLxa\n" +
    "CsJTjEstix35W0/aP7aJKdrt8DhevifZNzWHdNrYoIQOz4EQ/ORzqcxtrz7VUAz/\n" +
    "2B+eVX49DbqvwRi9qs5Ma1CAKcuHDCWH1/HXo/xc10cYp8QATK0pQVxuC1Mqu16m\n" +
    "FQIDAQAB\n" +
    "-----END PUBLIC KEY-----\n";

export const firebaseCustomToken = functions.https.onRequest((request, response) => {
    corsHandler(request, response, () => {
        const encodedAuth0IdToken = request.body;
        const auth0IdToken = jwt.verify(encodedAuth0IdToken, publicKey) as IAuth0IdToken;

        if (auth0IdToken && auth0IdToken.sub) {
            if (firebaseAdmin.apps.length === 0) {
                firebaseAdmin.initializeApp();
            }
            const uid: string = auth0IdToken.sub;
            const auth = firebaseAdmin.auth();
            auth.createCustomToken(uid)
                .then(customToken => {
                    response.send({
                        firebaseToken: customToken
                    });
                })
                .catch(error => {
                    throw new functions.https.HttpsError('internal', error);
                });
        } else {
            throw new functions.https.HttpsError('invalid-argument', 'Invalid Auth0 IdToken.');
        }
    });
});

interface IAuth0IdToken {
    given_name: string;
    family_name: string;
    nickname: string;
    name: string;
    picture: string;
    locale: string;
    updated_at: string;
    email: string;
    email_verified: boolean;
    iss: string;
    sub: string;
    aud: string;
    iat: number;
    exp: number;
    nonce: string;
}
