CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 3715;
	title = 371502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 371507;
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
				npcidx = "1362101;";
			}
			CDboTSCheckClrQst
			{
				and = "3713;";
				flink = 1;
				flinknextqid = "3716;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 3715;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSRcvSvrEvt
			{
				id = 1207;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 3715;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 371509;
				ctype = 1;
				idx = 1362101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 371508;
				gtype = 2;
				area = 371501;
				goal = 371504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 371505;
				stype = 2;
				taid = 1;
				title = 371502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 371514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 371501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "8;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1163102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 371508;
			gtype = 2;
			oklnk = 2;
			area = 371501;
			goal = 371504;
			sort = 371505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 371502;
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
			prelnk = "4;2;";
		}
	}
}

