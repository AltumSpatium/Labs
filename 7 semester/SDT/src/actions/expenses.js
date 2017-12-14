import firebaseApp from 'utils/firebase';
import moment from 'moment';
import {
    success
} from 'actions/default';
import {
    GET_ALL_EXPENSES,
    ADD_EXPENSE,
    UPDATE_EXPENSE,
    REMOVE_EXPENSE,
    CLEAR_EXPENSES
} from 'constants/expenses';

const getAllExpensesSuccess = success(GET_ALL_EXPENSES);
const addExpenseSuccess = success(ADD_EXPENSE);
const updateExpenseSuccess = success(UPDATE_EXPENSE);
const removeExpenseSuccess = success(REMOVE_EXPENSE);

export const getAllExpenses = () => dispatch => {
    const user = JSON.parse(localStorage['stUser']);
    const expensesRef = firebaseApp.database().ref('expenses/' + user.uid);
    
    return expensesRef.once('value', snapshot => {
        const expenses = [];
        snapshot.forEach(childSnapshot => {
            const expense = {
                id: childSnapshot.key,
                ...childSnapshot.val()
            };
            if (new Date(expense.date) <= moment().toDate())
                expenses.push(expense);
        });
        return dispatch(getAllExpensesSuccess(expenses));
    });
};

export const addExpense = newExpense => dispatch => dispatch(addExpenseSuccess(newExpense));
export const updateExpense = updatedExpense => dispatch => dispatch(updateExpenseSuccess(updatedExpense));
export const removeExpense = id => dispatch => dispatch(removeExpenseSuccess(id));
export const clearExpenses = () => dispatch => dispatch({type: CLEAR_EXPENSES});
