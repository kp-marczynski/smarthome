import {Component} from '@angular/core';
import {AuthService} from '../services/auth.service';
import {FirebaseAuthService} from '../services/firebase-auth.service';
import {HttpClient} from '@angular/common/http';
import {AngularFireDatabase, AngularFireList} from '@angular/fire/database';

@Component({
    selector: 'app-home',
    templateUrl: 'home.page.html',
    styleUrls: ['home.page.scss'],
})
export class HomePage {

    public queue: AngularFireList<any>;

    constructor(
        public auth: AuthService,
        private firebaseAuth: FirebaseAuthService,
        private httpClient: HttpClient,
        private angularFireDatabase: AngularFireDatabase
    ) {
        this.auth.userProfile$.subscribe(res => {
            if (res) {
                console.log(res);
                this.auth.getIdToken().then(idToken => {
                    console.log(idToken);
                    httpClient.post('https://us-central1-kpmarczynski-smarthome.cloudfunctions.net/firebaseCustomToken', idToken)
                        .subscribe((firebaseCustomToken: any) => {
                            console.log(firebaseCustomToken);
                            this.firebaseAuth.signInWithCustomToken(firebaseCustomToken.firebaseToken).then(res2 => console.log(res2));
                        });
                });

            }
        });
        this.queue = this.angularFireDatabase.list('/queue');
    }

    toggleGarage() {
        console.log(this.firebaseAuth.user);
        this.queue.push({
            user: this.firebaseAuth.user.uid,
            device: 'garage',
            timestamp: Date.now()
        });
    }
}
