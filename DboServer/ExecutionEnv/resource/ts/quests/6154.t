CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6154;
	title = 1100608;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 16590;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100607;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 7;
			desc = 1100764;
			nextlnk = 255;
			rwdtbl = 1161801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 254;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckClrQst
			{
				and = "6154;";
				flink = 0;
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
				cont = 1100610;
				gtype = -1;
				area = 1100800;
				goal = 1100609;
				m0fz = "4164.000000";
				m0widx = 1;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "5735.000000";
				m0ttip = 1100608;
				rwd = 7;
				taid = 1;
				title = 1100608;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100605;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = 4072102;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 9;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1100606;
			gtype = 2;
			oklnk = 2;
			area = 1100800;
			goal = 1100609;
			sort = 105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 7;
			title = 1100608;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
	}
}

