import pickle
import hashlib
import os
from functools import wraps
from typing import Any, Callable, Dict, Tuple


class PickleCache:
	def __init__(self, cache_file="cache.pkl"):
		self.cache_file = cache_file
		self._cache = self._load_cache()
	
	def _load_cache(self):
		if os.path.exists(self.cache_file):
			try:
				with open(self.cache_file, "rb") as f:
					return pickle.load(f)
			except (pickle.PickleError, EOFError):
				pass
		return {}
	
	def _save_cache(self):
		try:
			with open(self.cache_file, "wb") as f:
				pickle.dump(self._cache, f)
		except (pickle.PickleError, OSError):
			pass
	
	def _serialize_for_hash(self, obj):
		if isinstance(obj, (str, int, float, bool, type(None))):
			return str(obj)
		elif isinstance(obj, (list, tuple)):
			return f"[{','.join(self._serialize_for_hash(item) for item in obj)}]"
		elif isinstance(obj, dict):
			items = sorted((k, self._serialize_for_hash(v)) for k, v in obj.items())
			return f"{{{','.join(f'{k}:{v}' for k, v in items)}}}"
		elif hasattr(obj, '__dict__'):
			return self._serialize_for_hash(obj.__dict__)
		else:
			return str(obj)
	
	def _generate_key(self, func_name, args, kwargs):
		normalized = {'__args__': args} if args else {}
		if kwargs:
			normalized.update(sorted(kwargs.items()))
		
		content = f"{func_name}:{self._serialize_for_hash(normalized)}"
		return hashlib.sha256(content.encode()).hexdigest()
	
	def cached(self, func):
		@wraps(func)
		def wrapper(*args, **kwargs):
			cache_key = self._generate_key(func.__name__, args, kwargs)
			
			if cache_key in self._cache:
				return self._cache[cache_key]
			
			result = func(*args, **kwargs)
			self._cache[cache_key] = result
			self._save_cache()
			
			return result
		
		return wrapper
