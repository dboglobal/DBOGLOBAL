CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 56;
	title = 5602;

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
				eitype = 3;
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 5609;
				ctype = 1;
				idx = 3331201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 5608;
				area = 5601;
				goal = 5604;
				m0fz = "4191.000000";
				m0widx = 1;
				scitem = -1;
				sort = 5605;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "5792.000000";
				m0ttip = 5615;
				rwd = 100;
				taid = 1;
				title = 5602;
				gtype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 366;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 5614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 5601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 5607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckClrQst
			{
				and = "54;";
				flink = 1;
				flinknextqid = "57;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 5608;
			gtype = 2;
			oklnk = 2;
			area = 5601;
			goal = 5604;
			sort = 5605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 5602;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 366;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 366;
			}
		}
	}
}

