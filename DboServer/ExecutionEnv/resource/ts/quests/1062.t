CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1062;
	title = 106202;

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
				conv = 106209;
				ctype = 1;
				idx = 8031401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 106208;
				gtype = 3;
				area = 106201;
				goal = 106204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 106205;
				stype = 2;
				taid = 1;
				title = 106202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 106214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 106201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142301;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 106207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "8031401;";
			}
			CDboTSCheckClrQst
			{
				and = "1071;";
				flink = 1;
				flinknextqid = "1880;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 106208;
			gtype = 3;
			oklnk = 2;
			area = 106201;
			goal = 106204;
			sort = 106205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 106202;
		}
	}
}

