function updateFirebaseUser(user, context, callback) {
    const request = require('request');

    const baseURL = configuration.FIREBASE_URL;
    const secret = configuration.FIREBASE_SECRET;
    const fb_id = Buffer.from(user.user_id).toString('base64');

    const fbIdentity = {
        identity: {
            user_id: user.user_id,
            email: user.email,
            name: user.name,
            nickname: user.nickname,
            picture: user.picture
        }
    };

    const putURL = baseURL + '/users/' + fb_id + '.json?auth=' + secret;
    request.put({
            url: putURL,
            json: fbIdentity
        },
        function(err, response, body) {
            if (err) return callback(err);
            return callback(null, user, context);
        });
}
