import {
    GET_ALL_INCOMES,
    ADD_INCOME,
    UPDATE_INCOME,
    REMOVE_INCOME,
    CLEAR_INCOMES
} from 'constants/income';

const initialState = {
    incomes: []
};

const income = (state = initialState, action) => {
    switch (action.type) {
        case GET_ALL_INCOMES:
            return { ...state, incomes: action.payload };
        case ADD_INCOME:
            const incomes = state.incomes.slice();
            incomes.push(action.payload);
            return { ...state, incomes };
        case UPDATE_INCOME:
            const updatedIncome = action.payload;
            return { ...state, incomes: state.incomes.map(
                item => item.id === updatedIncome.id ? updatedIncome : item) };
        case REMOVE_INCOME:
            const id = action.payload;
            return { ...state, incomes: state.incomes.filter(item => item.id !== id) };
        case CLEAR_INCOMES:
                return { ...state, incomes: [] };
        default:
            return state;
    }
};

export default income;
