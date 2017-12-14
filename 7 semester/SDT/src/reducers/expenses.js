import {
    GET_ALL_EXPENSES,
    ADD_EXPENSE,
    UPDATE_EXPENSE,
    REMOVE_EXPENSE,
    CLEAR_EXPENSES
} from 'constants/expenses';

const initialState = {
    expenses: []
};

const expenses = (state = initialState, action) => {
    switch (action.type) {
        case GET_ALL_EXPENSES:
            return { ...state, expenses: action.payload };
        case ADD_EXPENSE:
            const expenses = state.expenses.slice();
            expenses.push(action.payload);
            return { ...state, expenses };
        case UPDATE_EXPENSE:
            const updatedExpense = action.payload;
            return { ...state, expenses: state.expenses.map(
                item => item.id === updatedExpense.id ? updatedExpense : item) };
        case REMOVE_EXPENSE:
            const id = action.payload;
            return { ...state, expenses: state.expenses.filter(item => item.id !== id) };
        case CLEAR_EXPENSES:
                return { ...state, expenses: [] };
        default:
            return state;
    }
};

export default expenses;
