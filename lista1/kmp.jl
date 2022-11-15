
function compute_prefix(pattern)
    m = length(pattern)
    p = zeros(Int, m)
    k = 0
    
    for q in 2:m
        while k > 0 && pattern[k+1] != pattern[q]
            k = p[k]
        end

        if pattern[k+1] == pattern[q]
            k = k+1
        end

        p[q] = k
    end

    return p
end


function knuth_morris(text, pattern)
    n = length(text)
    m = length(pattern)
    p = compute_prefix(pattern)
    q = 0

    for i in 1:n
        while q > 0 && pattern[q+1] != text[i]
            q = p[q]
        end

        if pattern[q+1] == text[i]
            q = q+1
        end
        
        if q == m
            s = i-m
            println("Wzorzec wystepuje z przesunięciem $s")
            q = p[q]
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

    knuth_morris(content[1], pattern)

    print("\n=============================\n\n\n")

    close(file)    
end

main()