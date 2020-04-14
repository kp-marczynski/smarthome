import {Component} from '@angular/core';
import {AuthService} from '../services/auth.service';
import {FirebaseAuthService} from '../services/firebase-auth.service';
import {HttpClient} from '@angular/common/http';

@Component({
    selector: 'app-home',
    templateUrl: 'home.page.html',
    styleUrls: ['home.page.scss'],
})
export class HomePage {

    constructor(private auth: AuthService, private firebaseAuth: FirebaseAuthService, private httpClient: HttpClient) {
        this.auth.userProfile$.subscribe(res => {
            if (res) {
                console.log(res);
                this.auth.getIdToken().then(idToken => {
                    httpClient.post('https://us-central1-kpmarczynski-smarthome.cloudfunctions.net/getToken', idToken).subscribe(funResult => console.log(funResult))
                    // this.firebaseAuth.signInWithCustomToken(idToken).then(res2 => console.log(res2));
                });

            }
        });
    }

}
