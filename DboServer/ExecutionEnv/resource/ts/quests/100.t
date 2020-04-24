CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 100;
	title = 10002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 10009;
				ctype = 1;
				idx = 7611201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 10008;
				gtype = 1;
				area = 10001;
				goal = 10004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 10005;
				stype = 2;
				taid = 1;
				title = 10002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 10007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 13;
			}
			CDboTSCheckClrQst
			{
				and = "99;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7611201;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 10008;
			gtype = 1;
			oklnk = 2;
			area = 10001;
			goal = 10004;
			sort = 10005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 10002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 10014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 10001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142102;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

