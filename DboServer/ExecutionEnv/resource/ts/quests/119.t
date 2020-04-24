CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 119;
	title = 11902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 103;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActQItem
			{
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 102;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 103;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 0;
				iidx = -1;
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
			prelnk = "102;103;";
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
			stdiag = 11907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "2801201;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "120;";
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
				conv = 11909;
				ctype = 1;
				idx = 2801201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 9611100;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 11908;
				m1fx = "5795.910156";
				m1fy = "0.000000";
				sort = 11905;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "3038.699951";
				grade = 132203;
				m0fx = "5536.580078";
				m0ttip = 11915;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 11902;
				gtype = 2;
				area = 11901;
				goal = 11904;
				m0fz = "3069.050049";
				m0widx = 1;
				m1ttip = 11915;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 11914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 11901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 0;
				iidx = -1;
			}
			CDboTSClickNPC
			{
				npcidx = "2801201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 11908;
			gtype = 2;
			oklnk = 2;
			area = 11901;
			goal = 11904;
			sort = 11905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 11902;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

