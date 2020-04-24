CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11119;
	title = 1100845;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;3;";

			CDboTSActBroadMsg
			{
				dt = 5;
				et = 2;
				oidx = 7511101;
				oqtidx = 1100886;
				ot = 0;
				uisdt = 2;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 1;
				uidt = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 4;
			prelnk = "5;";

			CDboTSActETimerE
			{
				taeid = 1;
				taid = 1;
				tceid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 11220;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 1;
				tgid = 1;
				time = 180000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 1100889;
				gtype = 3;
				area = 1100807;
				goal = 1100888;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100887;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 254;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 254;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100890;
			gt = 0;
			oi = 2951201;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;4;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 11200;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 1;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100885;
			gt = 0;
			oi = 7111404;
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
}

