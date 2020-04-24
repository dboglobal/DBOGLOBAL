CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 580;
	title = 58002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSBindStone
			{
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 58009;
				ctype = 1;
				idx = 7711101;
				taid = 2;
			}
			CDboTSActOpenWindow
			{
				show = 1;
				taid = 3;
				tblidx = 20003;
				wtype = 5;
			}
			CDboTSActRegQInfo
			{
				cont = 58008;
				area = 58001;
				goal = 58004;
				m0fz = "-3915.570068";
				m0widx = 1;
				scitem = -1;
				sort = 58005;
				stype = 64;
				m0fy = "-96.580002";
				m0pat = 2;
				grade = 132203;
				m0fx = "5818.250000";
				m0ttip = 58015;
				rwd = 100;
				taid = 1;
				title = 58002;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 58014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 58001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7711101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 58007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "7711101;";
			}
			CDboTSCheckClrQst
			{
				and = "403;";
				flink = 1;
				flinknextqid = "1930;1931;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 58008;
			gtype = 1;
			oklnk = 2;
			area = 58001;
			goal = 58004;
			sort = 58005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 58002;
		}
	}
}

