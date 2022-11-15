
function transition_function(pattern, alphabet)
    m = length(pattern)
    k = 0
    delta_dict = Dict()

    for q in 0:m
        for letter in alphabet
            k = min(m, q+1)

            while !endswith((pattern[1:q] * letter), pattern[1:k])
                k = k-1
            end
            
            push!(delta_dict, (q, letter) => k) 
        end
    end
    
    return delta_dict
end


function fa_matcher(text, delta_dict, pattern)
    n = length(text)
    m = length(pattern)
    q = 0

    for i in 1:n
        q = get(delta_dict, (q, text[i]), 0)

        if q == m
            s = i-m
            println("Wzorzec występuje z przesunięciem $s")
        end
    end
end


function main()
    if length(ARGS) != 2
        println("Nieprawidłowa liczba argumentów")
        return -1
    end

    pattern = ARGS[1]
    filename = ARGS[2]

    file = open("$filename", "r")
    content = readlines(file)
    
    print("\n\n=============================\n")
    print("\nSzukany wzorzec: $pattern\n")
    print("\nTekst: $(content[1])\n")
    print("\n=============================\n\n")

    delta = transition_function(pattern, content[1])
    fa_matcher(content[1], delta, pattern)

    print("\n=============================\n\n\n")

    close(file)    
end

main()

