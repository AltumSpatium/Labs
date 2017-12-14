import firebaseApp from 'utils/firebase';
import {
    success
} from 'actions/default';
import {
    GET_ALL_ACCOUNTS,
    ADD_ACCOUNT,
    UPDATE_ACCOUNT,
    REMOVE_ACCOUNT,
    CLEAR_ACCOUNTS
} from 'constants/account';

const getAllAccountsSuccess = success(GET_ALL_ACCOUNTS);
const addAccountSuccess = success(ADD_ACCOUNT);
const updateAccountSuccess = success(UPDATE_ACCOUNT);
const removeAccountSuccess = success(REMOVE_ACCOUNT);

export const getAllAccounts = () => dispatch => {
    const user = JSON.parse(localStorage['stUser']);
    const accountsRef = firebaseApp.database().ref('accounts/' + user.uid);
    
    return accountsRef.once('value', snapshot => {
        const accounts = [];
        snapshot.forEach(childSnapshot => {
            const account = {
                id: childSnapshot.key,
                ...childSnapshot.val()
            };
            accounts.push(account);
        });
        return dispatch(getAllAccountsSuccess(accounts));
    });
};

export const addAccount = newAccount => dispatch => dispatch(addAccountSuccess(newAccount));
export const updateAccount = updatedAccount => dispatch => dispatch(updateAccountSuccess(updatedAccount));
export const removeAccount = id => dispatch => dispatch(removeAccountSuccess(id));
export const clearAccounts = () => dispatch => dispatch({type: CLEAR_ACCOUNTS});
