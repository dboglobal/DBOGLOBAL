CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2918;
	title = 291802;

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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 291809;
				ctype = 1;
				idx = 4531103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 1;
				idx0 = 1517;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1518;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 291808;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 291805;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2164.500000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 291801;
				goal = 291804;
				m0fz = "-3296.429932";
				m0widx = 1;
				m1ttip = 291816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-3348.790039";
				m3ttip = 291815;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3488.979980";
				m2fx = "2329.290039";
				m2ttip = 291815;
				m0fx = "2192.070068";
				m0ttip = 291816;
				m3pat = 2;
				title = 291802;
				m1fx = "2316.739990";
				m2fz = "-3428.270020";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 291814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 291801;
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
				npcidx = "4531103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 291808;
			gtype = 2;
			oklnk = 2;
			area = 291801;
			goal = 291804;
			sort = 291805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 291802;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 291807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "4531103;";
			}
			CDboTSCheckClrQst
			{
				and = "2913;";
				flink = 1;
				flinknextqid = "2919;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 1518;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1517;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 291807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

