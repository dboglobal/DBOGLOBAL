CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 24;
	rq = 0;
	tid = 3818;
	title = 381802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1653;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 381807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "1161101;";
			}
			CDboTSCheckClrQst
			{
				and = "3745;";
				flink = 0;
				flinknextqid = "3746;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 381809;
				ctype = 1;
				idx = 1161101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1653;
				cnt0 = 15;
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
				cont = 381808;
				m1fx = "-4985.799805";
				m1fy = "0.000000";
				sort = 381805;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-92.580002";
				grade = -1;
				m0fx = "-4907.359863";
				m0ttip = 381815;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				title = 381802;
				gtype = 2;
				area = 381801;
				goal = 381804;
				m0fz = "-4646.000000";
				m0widx = 15;
				m1ttip = 381815;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 381814;
			nextlnk = 3;
			rwdexp = 0;
			rwdtbl = 381801;
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
				npcidx = "1161101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 381808;
			gtype = 2;
			oklnk = 2;
			area = 381801;
			goal = 381804;
			sort = 381805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 381802;
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
				iprob0 = "1.000000";
				iidx0 = 1653;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 381807;
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
	}
}

