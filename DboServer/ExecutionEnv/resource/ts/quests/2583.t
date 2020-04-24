CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2583;
	title = 258302;

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
				eitype = 1;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 258309;
				ctype = 1;
				idx = 2806101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 12;
				eitype = 1;
				idx0 = 1242;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1243;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 258308;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "-352.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "-1305.000000";
				m4ttip = 258316;
				area = 258301;
				goal = 258304;
				m0fz = "-3599.399902";
				m0widx = 1;
				m1ttip = 258316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-3936.000000";
				m3ttip = 258316;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "-3518.000000";
				m2fx = "-990.000000";
				m2ttip = 258316;
				m4widx = 1;
				m0fx = "-1161.810059";
				m0ttip = 258315;
				m3pat = 2;
				m4pat = 2;
				title = 258302;
				m1fx = "-1303.000000";
				m2fz = "-3584.000000";
				sort = 258305;
				m4fz = "-3949.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 258308;
			gtype = 3;
			oklnk = 2;
			area = 258301;
			goal = 258304;
			sort = 258305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 258302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 258314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 258301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "2806101;";
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
			stdiag = 258307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2582;";
				flink = 1;
				flinknextqid = "2584;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2806101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 258307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 1243;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1242;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

