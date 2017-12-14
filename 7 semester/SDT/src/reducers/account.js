import {
    GET_ALL_ACCOUNTS,
    ADD_ACCOUNT,
    UPDATE_ACCOUNT,
    REMOVE_ACCOUNT,
    CLEAR_ACCOUNTS
} from 'constants/account';

const initialState = {
    accounts: []
};

const auth = (state = initialState, action) => {
    switch (action.type) {
        case GET_ALL_ACCOUNTS:
            return { ...state, accounts: action.payload };
        case ADD_ACCOUNT:
            const accounts = state.accounts.slice();
            accounts.push(action.payload);
            return { ...state, accounts };
        case UPDATE_ACCOUNT:
            const updatedAccount = action.payload;
            return { ...state, accounts: state.accounts.map(
                item => item.id === updatedAccount.id ? updatedAccount : item) };
        case REMOVE_ACCOUNT:
            const id = action.payload;
            return { ...state, accounts: state.accounts.filter(item => item.id !== id) };
        case CLEAR_ACCOUNTS:
                return { ...state, accounts: [] };
        default:
            return state;
    }
};

export default auth;
