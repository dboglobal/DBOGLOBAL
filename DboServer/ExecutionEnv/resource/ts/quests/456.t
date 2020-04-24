CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 456;
	title = 45602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;98;100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 45607;
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
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "4151201;";
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 252;
			cid = 3;
			lilnk = 255;
			ndesc0 = 45631;
			uspt = -1;
			desc = 45630;
			nid2 = 101;
			type = 0;
			ust = 0;
			idx = 4151201;
			ndesc2 = 45633;
			nid1 = 100;
			prelnk = "4;";
			ndesc1 = 45632;
			nid0 = 98;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 45608;
				gtype = 1;
				area = 45601;
				goal = 45604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 45605;
				stype = 1;
				taid = 1;
				title = 45602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 45634;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 45602;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151201;";
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 45614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 45601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151201;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 98;
			rwdzeny = 0;
			desc = 45634;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 45603;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 45608;
			gtype = 1;
			oklnk = 2;
			area = 45601;
			goal = 45604;
			sort = 45605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 45602;
		}
	}
}

