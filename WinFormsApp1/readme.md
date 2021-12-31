## [grpc](https://grpc.io/docs/languages/csharp/dotnet/)
## [aspnetcore](https://docs.microsoft.com/en-us/aspnet/core/grpc/?view=aspnetcore-6.0)
## [tutorial](https://docs.microsoft.com/en-us/aspnet/core/tutorials/grpc/grpc-start?view=aspnetcore-6.0&tabs=visual-studio)
```
PMC option to install packages
From Visual Studio, select Tools > NuGet Package Manager > Package Manager Console

From the Package Manager Console window, run cd GrpcGreeterClient to change directories to the folder containing the GrpcGreeterClient.csproj files.

Run the following commands:

PowerShell

Copy
Install-Package Grpc.Net.Client
Install-Package Google.Protobuf
Install-Package Grpc.Tools

```

### Right-click the project and select Edit Project File.
```
<ItemGroup>
  <Protobuf Include="Protos\greet.proto" GrpcServices="Client" />
</ItemGroup>


Build the client project to create the types in the GrpcGreeter namespace. 
The GrpcGreeter types are generated automatically by the build process.
自动在obj目录中生成接口类
```