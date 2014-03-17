angular.module('codepuppy').controller('AnnouncementListCtrl', ['$scope', '$routeParams', function($scope, $routeParams) {
	$scope.sortByField = "date_added";
	$scope.reverse = true;
	$scope.newAnnouncementPath = '/#/courses/' + $routeParams.courseID + '/newAnnouncement';
	$scope.announcements = [{date_added: '2012-08-14', subject: 'Remember to buy a textbook people', body: 'BLAH BLAH BLAH'}, {date_added: '2012-08-17', subject: 'First sections tomorrow', body: 'BLAH FIRST BLAH'}, ];
	$scope.selectedAnnouncement = null;

	$scope.announcementSelected = function(announcement) {
		$scope.selectedAnnouncement = announcement;
	};
}]);