CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1629;
	title = 162902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 162908;
			gtype = 2;
			oklnk = 2;
			area = 162901;
			goal = 162904;
			sort = 162905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 162902;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 162914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 162901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111103;";
			}
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
			stdiag = 162907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "6111103;";
			}
			CDboTSCheckClrQst
			{
				and = "1628;";
				flink = 1;
				flinknextqid = "1630;";
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
				conv = 162909;
				ctype = 1;
				idx = 6111103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 4261101;
				cnt0 = 7;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 4262101;
				taid = 3;
				esctype = 0;
				idx2 = 4263100;
			}
			CDboTSActRegQInfo
			{
				cont = 162908;
				m1fx = "-1788.000000";
				m2fz = "487.000000";
				m1fy = "0.000000";
				sort = 162905;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 162901;
				goal = 162904;
				m0fz = "487.000000";
				m0widx = 1;
				m1ttip = 162916;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "487.000000";
				m2fx = "-1788.000000";
				m2ttip = 162917;
				m0fx = "-1788.000000";
				m0ttip = 162915;
				title = 162902;
			}
		}
	}
}

