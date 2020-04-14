import * as functions from 'firebase-functions';
import * as jwt from 'jsonwebtoken';
import * as fs from 'fs';
// import * as firebaseAdmin from 'firebase-admin';
// Start writing Firebase Functions
// https://firebase.google.com/docs/functions/typescript

export const helloWorld = functions.https.onRequest((request, response) => {
    response.send("Hello from Firebase!");
});

export const getToken = functions.https.onRequest((request, response) => {
    // firebaseAdmin.initializeApp({credential: firebaseAdmin.credential.cert()})
    console.log(request.body);
    const theJwt = fs
        .readFileSync("auth0PublicKey.txt")
        .toString()
        .replace(/\n$/, '');

    const decoded = jwt.verify(request.body, theJwt);

    response.set('Access-Control-Allow-Origin', '*');
    if (decoded) {
        response.send(decoded);
    } else {
        response.send('failed');
    }
});

