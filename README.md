# 5 guys (but only 3 are guys) -COS214 :sparkles: :sparkles:
## TIVIDY workflow management system

# Content

- [Important Documents](#documents)
- [Team members + responsibilities](#members)


# Introduction

TIVIDY is an intigrated workflow-management system that uses a combination of destinct and meaningfull GoF design patterns

# Documents

# How to demo

In Project directory run:
  - `make` to build, then `make run` to start the program
  - `make test` to run the unit tests
  - `make docs` to generate the Doxygen documentation (output in `docs/html/index.html`)
Using Docker Compose:   
  - Run main:   
    docker-compose up --build   
  - Run Valgrind:   
    docker-compose up --profile tools run --rm workflow_valgrind   
  - Run GDB:   
    docker-compose up --profile tools run --rm workflow_gdb   
to clear:
  - `make clean`
  - `docker-compose down`
# Members

<table>
  <tr>
    <th>Members</th>
    <th>Roles</th>
  </tr>
  <tr>
    <td>Jayden Smith</td>
    <td>
      <ul>
        <li>Facade design pattern</li>
        <li>Memento design pattern</li>
        <li>Unit Testing</li>
        <li>Doxygen</li>
      </ul>
    </td>
  </tr>
  <tr>
    <td>Leanne van der Horst</td>
    <td>
      <ul>
        <li>Observer design pattern</li>
        <li>Iterator design pattern</li>
        <li>TUI</li>
      </ul>
    </td>
  </tr>
  <tr>
    <td>Yash Ghela</td>
    <td>
      <ul>
        <li>Chain of responsibility design pattern</li>
        <li>Participant composite</li>
        <li>Work handler startegy</li>
      </ul>
    </td>
  </tr>
  <tr>
    <td>Rei</td>
    <td>
      <ul>
        <li>Decorator design pattern</li>
        <li>Docker compose + Dockerfile</li>
      </ul>
    </td>
  </tr>
  <tr>
    <td>Declan Mc Cullough</td>
    <td>
      <ul>
        <li>Workflow Composite</li>
        <li>State design pattern</li>
      </ul>
    </td>
  </tr>
</table>




