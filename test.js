import test from 'ava';
import libuiRichtexteditor from './';

test('it work!', t => {
	const result = libuiRichtexteditor();
	t.is(result, 42);
});
