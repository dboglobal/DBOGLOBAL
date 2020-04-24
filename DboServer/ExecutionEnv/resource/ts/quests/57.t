CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 57;
	title = 5702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 4;
			prelnk = "9;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 5727;
			}
		}
		CDboTSContGCond
		{
			cid = 9;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 10;

			CDboTSColRgn
			{
				widx = 1;
				cct = 1;
				crt = 0;
				r = "50.000000";
				x = "5773.700195";
				z = "4188.709961";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "10;3;";
			nolnk = 255;
			rm = 1;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 5708;
			gtype = 1;
			oklnk = 2;
			area = 5701;
			goal = 5704;
			sort = 5705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 5702;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 102;
			rwdzeny = 0;
			desc = 5724;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 5703;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 5725;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 5702;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 9;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 5709;
				ctype = 1;
				idx = 3331201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 5708;
				area = 5701;
				goal = 5704;
				m0fz = "4188.709961";
				m0widx = 1;
				scitem = -1;
				sort = 5705;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "5773.700195";
				m0ttip = 5715;
				rwd = 100;
				taid = 1;
				title = 5702;
				gtype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 5707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckClrQst
			{
				and = "56;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 4;
			cid = 3;
			lilnk = 255;
			ndesc0 = 5723;
			uspt = -1;
			desc = 5720;
			nid2 = 102;
			type = 0;
			ust = 0;
			idx = 3331201;
			ndesc2 = 5721;
			nid1 = 101;
			prelnk = "4;";
			ndesc1 = 5722;
			nid0 = 100;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;101;102;";
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
			desc = 5726;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 5701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3331201;";
			}
		}
	}
}

