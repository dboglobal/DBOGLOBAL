CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 695;
	title = 69502;

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
				conv = 69509;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 69508;
				gtype = 1;
				area = 69501;
				goal = 69504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 69505;
				stype = 2;
				taid = 1;
				title = 69502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 69508;
			gtype = 1;
			oklnk = 2;
			area = 69501;
			goal = 69504;
			sort = 69505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 69502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 69514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 69501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351202;";
			}
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
			stdiag = 69507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "696;";
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
	}
}

