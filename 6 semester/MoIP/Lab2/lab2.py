from lsb import LSB
from patchwork import Patchwork
import os


def main():
	while True:
		algorithm = get_algorithm()
		print()
		if algorithm == '0':
			return
		elif algorithm == '1':
			while True:
				option = get_choosen_option("Hide", "Reveal")
				print()
				if option == '0':
					break
				elif option in ('1', '2'):
					in_image = get_image_path("input")
					if option == '1':
						out_image = get_image_path("output")
						out_image = None if out_image == "" else out_image

						message = get_user_input("message")
						l = LSB(in_image, outImage=out_image)
						l.hide(message)
						print("Done")
						return
					elif option == '2':
						l = LSB(in_image)
						print("Message:", l.reveal())
						return
				else:
					print("Wrong input, try again...")
		elif algorithm == '2':
			while True:
				option = get_choosen_option("Insert", "Find")
				print()
				if option == '0':
					break
				elif option in ('1', '2'):
					key = get_user_input("key")
					in_image = get_image_path("input")
					if option == '1':
						out_image = get_image_path("output")
						out_image = None if out_image == "" else out_image

						p = Patchwork(in_image, 3, 10000, outImage=out_image)
						p.insert_watermark(key)
						print("Done")
						return
					elif option == '2':
						p = Patchwork(in_image, 3, 10000)
						if p.find_watermark(key):
							print('Digital watermark is present')
						else:
							print('Digital watermark is absent')
						return
				else:
					print("Wrong input, try again...")
		else:
			print("Wrong input, try again...")


def get_algorithm():
	print("Choose an algorithm:")
	print("1 -- LSB")
	print("2 -- Patchwork")
	print("0 -- Exit")
	algorithm = input(">>")

	return algorithm


def get_choosen_option(*variants):
	print("Choose an option:")
	for i in range(0, len(variants)):
		print(str(i + 1), "--", variants[i])
	print("0 -- Exit")
	option = input(">>")

	return option


def get_user_input(type):
	print("Enter your", type, ":")
	return input(">>")


def get_image_path(kind):
    while True:
        print("Enter path to", kind, "image:")
        path = input(">>")
        if kind == "input" and not os.path.isfile(path):
            print("No such image!")
            continue
        return path


if __name__ == "__main__":
	main()
