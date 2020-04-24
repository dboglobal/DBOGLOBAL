CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 462;
	title = 46202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 46209;
				ctype = 1;
				idx = 1654108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 46208;
				gtype = 1;
				area = 46201;
				goal = 46204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 46205;
				stype = 2;
				taid = 1;
				title = 46202;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 46207;
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
				npcidx = "1654108;";
			}
			CDboTSCheckClrQst
			{
				and = "461;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 46208;
			gtype = 1;
			oklnk = 2;
			area = 46201;
			goal = 46204;
			sort = 46205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 46202;
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
			desc = 46214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 46201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4261101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

