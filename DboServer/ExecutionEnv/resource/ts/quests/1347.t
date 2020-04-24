CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 5;
	rq = 0;
	tid = 1347;
	title = 134702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 1347;
				start = 1;
				taid = 2;
				uof = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4801100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 1347;
				start = 0;
				taid = 2;
				uof = 0;
			}
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 134708;
				area = 134701;
				goal = 134704;
				m0fz = "4252.000000";
				m0widx = 1;
				scitem = -1;
				sort = 134705;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "2751.000000";
				m0ttip = 134715;
				rwd = 100;
				taid = 1;
				title = 134702;
				gtype = 3;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 134707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSClickNPC
			{
				npcidx = "3331103;";
			}
			CDboTSCheckClrQst
			{
				and = "1346;";
				flink = 1;
				flinknextqid = "1348;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 134708;
			gtype = 3;
			oklnk = 2;
			area = 134701;
			goal = 134704;
			sort = 134705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 134702;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;2;";
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
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
			desc = 134714;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 134701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3331103;";
			}
		}
	}
}

