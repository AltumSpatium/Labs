import firebaseApp from 'utils/firebase';
import moment from 'moment';
import {
    success
} from 'actions/default';
import {
    GET_ALL_INCOMES,
    ADD_INCOME,
    UPDATE_INCOME,
    REMOVE_INCOME,
    CLEAR_INCOMES
} from 'constants/income';

const getAllIncomesSuccess = success(GET_ALL_INCOMES);
const addIncomeSuccess = success(ADD_INCOME);
const updateIncomeSuccess = success(UPDATE_INCOME);
const removeIncomeSuccess = success(REMOVE_INCOME);

export const getAllIncomes = () => dispatch => {
    const user = JSON.parse(localStorage['stUser']);
    const incomeRef = firebaseApp.database().ref('income/' + user.uid);
    
    return incomeRef.once('value', snapshot => {
        const incomes = [];
        snapshot.forEach(childSnapshot => {
            const income = {
                id: childSnapshot.key,
                ...childSnapshot.val()
            };
            if (new Date(income.date) <= moment().toDate())
                incomes.push(income);
        });
        return dispatch(getAllIncomesSuccess(incomes));
    });
};

export const addIncome = newIncome => dispatch => dispatch(addIncomeSuccess(newIncome));
export const updateIncome = updatedIncome => dispatch => dispatch(updateIncomeSuccess(updatedIncome));
export const removeIncome = id => dispatch => dispatch(removeIncomeSuccess(id));
export const clearIncomes = () => dispatch => dispatch({type: CLEAR_INCOMES});
