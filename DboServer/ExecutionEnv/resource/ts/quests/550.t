CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 550;
	title = 55002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 55031;
				ctype = 1;
				idx = 2611202;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 55007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 32;
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
			CDboTSCheckClrQst
			{
				and = "539;";
				flink = 1;
				flinknextqid = "551;";
				not = 0;
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
				conv = 55009;
				ctype = 1;
				idx = 2611101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 55008;
				area = 55001;
				goal = 55004;
				m0fz = "-1455.000000";
				m0widx = 1;
				scitem = -1;
				sort = 55005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "7186.000000";
				m0ttip = 55015;
				rwd = 100;
				taid = 1;
				title = 55002;
				gtype = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

			CDboTSSkillUse
			{
				idx = 520121;
				rp = 0;
				stype = 6;
				tidx = 2611202;
				ttype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 55014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 55001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 55008;
			gtype = 1;
			oklnk = 2;
			area = 55001;
			goal = 55004;
			sort = 55005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 55002;
		}
	}
}

